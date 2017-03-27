from random import shuffle

num = 1000000
with open('data2.txt', 'wb') as writer:
  nums = [i for i in range(num)]
  shuffle(nums)
  writer.write(' '.join([str(s) for s in nums]))
