# scripts/adaptive_frequency_hopping.py

import random
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

class AdaptiveFrequencyHopping:
    def __init__(self, min_freq, max_freq, num_channels):
        self.min_freq = min_freq
        self.max_freq = max_freq
        self.num_channels = num_channels
        self.channel_history = []
        self.channel_performance = {i: 1.0 for i in range(num_channels)}
        self.consecutive_failures = {i: 0 for i in range(num_channels)}
        self.jamming_threshold = 3  # Number of consecutive failures to consider a channel jammed
        logging.info(f"Initialized AdaptiveFrequencyHopping with {num_channels} channels from {min_freq} Hz to {max_freq} Hz")

    def get_next_frequency(self, noise_levels):
        logging.info(f"Current noise levels: {noise_levels}")
        
        # Identify potentially jammed channels
        jammed_channels = [i for i, failures in self.consecutive_failures.items() if failures >= self.jamming_threshold]
        
        # Weight channel selection based on past performance, current noise levels, and jamming status
        weights = []
        for i in range(self.num_channels):
            if i in jammed_channels:
                weights.append(0)  # Avoid jammed channels
            else:
                weights.append(self.channel_performance[i] / (1 + noise_levels[i]))
        
        if sum(weights) == 0:
            # All channels seem jammed, reset and try again
            self.reset_channel_performance()
            return self.get_next_frequency(noise_levels)
        
        best_channel = random.choices(range(self.num_channels), weights=weights)[0]
        self.channel_history.append(best_channel)
        
        selected_freq = self.min_freq + (self.max_freq - self.min_freq) * (best_channel / self.num_channels)
        logging.info(f"Selected channel {best_channel} with frequency {selected_freq} Hz")
        return selected_freq

    def update_channel_performance(self, channel, success):
        logging.info(f"Updating channel {channel} performance, success: {success}")
        if channel < 0 or channel >= self.num_channels:
            logging.error(f"Invalid channel {channel}")
            return
        
        old_performance = self.channel_performance[channel]
        if success:
            self.channel_performance[channel] *= 1.1
            self.consecutive_failures[channel] = 0
        else:
            self.channel_performance[channel] *= 0.9
            self.consecutive_failures[channel] += 1
        
        logging.info(f"Updated performance for channel {channel}: {old_performance:.2f} -> {self.channel_performance[channel]:.2f}")
        logging.info(f"Consecutive failures for channel {channel}: {self.consecutive_failures[channel]}")

    def simulate_noise(self):
        noise = [random.uniform(0, 1) for _ in range(self.num_channels)]
        logging.info(f"Simulated noise levels: {noise}")
        return noise

    def reset_channel_performance(self):
        self.channel_performance = {i: 1.0 for i in range(self.num_channels)}
        self.consecutive_failures = {i: 0 for i in range(self.num_channels)}
        logging.info("Reset channel performance due to potential jamming on all channels")

