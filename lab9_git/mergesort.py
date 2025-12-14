from heapq import merge

def merge_sort2(m):
    """Sort list, using two part merge sort"""
    if len(m) <= 1:
        return m
    middle = len(m) // 2
    right = m[middle:]
    left = m[:middle]
    right = merge_sort2(right)
    left = merge_sort2(left)
    return list(merge(right, left))

def merge_sort4(m):
    """Sort list, using four part merge sort"""
    if len(m) <= 4:
        return sorted(m)
    middle = len(m) // 2
    leftMiddle = middle // 2
    rightMiddle = middle + leftMiddle
    first = m[:leftMiddle]
    second = m[leftMiddle:middle]
    third = m[middle:rightMiddle]
    fourth = m[rightMiddle:]
    first = merge_sort4(first)
    second = merge_sort4(second)
    third = merge_sort4(third)
    fourth = merge_sort4(fourth)
    return list(merge(first, second, third, fourth))
