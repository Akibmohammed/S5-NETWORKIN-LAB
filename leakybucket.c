#include <stdio.h>

#define BUCKET_SIZE 10  // Max capacity of bucket
#define OUT_RATE 3      // Rate at which packets are sent

void leaky_bucket(int incoming_packets[], int n) {
    int bucket_content = 0;

    for (int i = 0; i < n; i++) {
        printf("\nTime %d:\n", i + 1);
        int incoming = incoming_packets[i];
        printf("Incoming packet size: %d\n", incoming);

        if (incoming + bucket_content > BUCKET_SIZE) {
            int dropped = (incoming + bucket_content) - BUCKET_SIZE;
            printf("Bucket overflow! Dropped %d packets.\n", dropped);
            bucket_content = BUCKET_SIZE;
        } else {
            bucket_content += incoming;
            printf("Bucket content after arrival: %d packets.\n", bucket_content);
        }

        if (bucket_content >= OUT_RATE) {
            bucket_content -= OUT_RATE;
            printf("Sent %d packets. Remaining in bucket: %d packets.\n", OUT_RATE, bucket_content);
        } else {
            printf("Sent %d packets. Bucket is now empty.\n", bucket_content);
            bucket_content = 0;
        }
    }

    // Send any remaining packets in the bucket
    while (bucket_content > 0) {
        printf("\nSending remaining packets...\n");
        if (bucket_content >= OUT_RATE) {
            bucket_content -= OUT_RATE;
            printf("Sent %d packets. Remaining in bucket: %d packets.\n", OUT_RATE, bucket_content);
        } else {
            printf("Sent %d packets. Bucket is now empty.\n", bucket_content);
            bucket_content = 0;
        }
    }
}

int main() {
    int incoming_packets[] = {4, 6, 8, 2, 7, 4};  // Packets arriving at each time step
    int n = sizeof(incoming_packets) / sizeof(incoming_packets[0]);

    printf("Leaky Bucket Simulation\n");
    leaky_bucket(incoming_packets, n);

    return 0;
}
