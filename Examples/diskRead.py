# Example of disk read in Python

numVoices = 100
batch = createABankOfVoices(numVoices)

def playSample(note, velocity):
    idx = batch.find(note)
    if idx != -1:
        batch[idx].playWithVelocity(velocity)
        return

    batch.popLast();
    batch.pushInFront(loadFromDisk(note))
    batch[0].playWithVelocity(velocity)
    return

for i in range(128):
    playSample(i, i)
