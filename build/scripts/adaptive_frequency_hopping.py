import random
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

class AdaptiveFrequencyHopping:
    def __init__(self, min_freq, max_freq, num_channels):
        self.min_freq = min_freq
        self.max_freq = max_freq
        self.num_channels = num_channels
        self.channel_history = []
        self.channel_performance = {i: 1.0 for i in range(num_channels)}  # Initialize with neutral performance
        logging.info(f"Initialized AdaptiveFrequencyHopping with {num_channels} channels from {min_freq} Hz to {max_freq} Hz")

    def get_next_frequency(self, noise_levels):
        logging.info(f"Current noise levels: {noise_levels}")
        # Weight channel selection based on past performance and current noise levels
        weights = [self.channel_performance[i] / (1 + noise_levels[i]) for i in range(self.num_channels)]
        
        best_channel = random.choices(range(self.num_channels), weights=weights)[0]
        self.channel_history.append(best_channel)
        
        selected_freq = self.min_freq + (self.max_freq - self.min_freq) * (best_channel / self.num_channels)
        logging.info(f"Selected channel {best_channel} with frequency {selected_freq} Hz")
        return selected_freq

    def update_channel_performance(self, channel, success):
        if channel < 0 or channel >= self.num_channels:
            logging.error(f"Invalid channel {channel}")
            return
        
        old_performance = self.channel_performance[channel]
        # Update channel performance based on transmission success
        if success:
            self.channel_performance[channel] *= 1.1  # Increase performance score for successful transmission
        else:
            self.channel_performance[channel] *= 0.9  # Decrease performance score for failed transmission
        logging.info(f"Updated performance for channel {channel}: {old_performance:.2f} -> {self.channel_performance[channel]:.2f}")

    def simulate_noise(self):
        noise = [random.uniform(0, 1) for _ in range(self.num_channels)]
        logging.info(f"Simulated noise levels: {noise}")
        return noise

