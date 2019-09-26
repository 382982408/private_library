#!/usr/bin/env python
# encoding: utf-8
import sys,os,re,operator,random,pprint

datasets = {'banala':{'long':400,'not_long':100,'sweet':350,'not_sweet':150,'yellow':450,'not_yellow':50},
            'orange':{'long':0,'not_long':300,'sweet':150,'not_sweet':150,'yellow':300,'not_yellow':0},
            'other_fruit':{'long':100,'not_long':100,'sweet':150,'not_sweet':50,'yellow':50,'not_yellow':150}
}
 
 
def count_total(data):
    '''
    function:计算各种水果的总数, return {‘banala’:500 ...}
    '''
    count = {}
    total = 0
    for fruit in data:
        '''因为水果要么甜要么不甜，可以用 这两种特征来统计总数'''
        count[fruit] = data[fruit]['sweet'] + data[fruit]['not_sweet']
        total += count[fruit]
    return count,total
 
#categories,simpleTotal = count_total(datasets)
#print(categories,simpleTotal)
###########################################################
 
def cal_base_rates(data):
    '''
    function:计算各种水果的先验概率 ,return {‘banala’:0.5 ...}
    '''
    categories,total = count_total(data)
    cal_base_rates = {}
    for label in categories:
        priori_prob = categories[label]/total
        cal_base_rates[label] = priori_prob
    return cal_base_rates
 
#Prio = cal_base_rates(datasets)
#print(Prio)
############################################################
 
def likelihold_prob(data):
    '''
    function:计算已知水果下各特征的概率分布（likelihood probabilities） {'banala':{'long':0.8}...}
    '''
    count,_ = count_total(data)
    likelihold = {}
    for fruit in data:
        '''创建一个临时的字典，临时存储各个特征值的概率'''
        attr_prob = {}
        for attr in data[fruit]:
            #计算各个特征值在已知水果下的概率
            attr_prob[attr] = data[fruit][attr]/count[fruit]
        likelihold[fruit] = attr_prob
    return likelihold
 
#LikeHold = likelihold_prob(datasets)
#print(LikeHold)
############################################################

def evidence_prob(data):
    '''
    function:计算各种特征的分布,return {'long':50%...}
    '''
    attrs = list(data['banala'].keys())
    count,total  = count_total(data)
    evidence_prob = {}
 
    #计算各种特征的概率
    for attr in attrs:
        attr_total = 0
        for fruit in data:
            attr_total += data[fruit][attr]
        evidence_prob[attr] = attr_total/total
    return evidence_prob


def evidence_prob_lzw(data):
    '''
    function:计算各种特征下各水果的分布,return {'long':50%...}
    '''
    attrs = list(data['banala'].keys())
    count,total  = count_total(data)
    evidence_prob = {}
 
    #计算各种特征的概率
    for attr in attrs:
        attr_total = 0
        for fruit in data:
            attr_total += data[fruit][attr]
        fruit_prob={}
        for fruit in data:
            fruit_prob[fruit] = data[fruit][attr]/attr_total
        evidence_prob[attr] = fruit_prob
    return evidence_prob
#Evidence_prob = evidence_prob(datasets)
#print(Evidence_prob)
##########################################################
#以上是训练数据用到的函数，即将数据转化为代码计算概率
##########################################################
 
class navie_bayes_classifier:
    '''
    初始化贝叶斯分类器,实例化时会调用__init__函数
    '''
    def __init__(self,data=datasets):
        self._data = datasets
        self._labels = list(self._data.keys())
        self._priori_prob = cal_base_rates(self._data)
        self._likelihold_prob = likelihold_prob(self._data)
        self._evidence_prob = evidence_prob(self._data)
        self._evidence_prob_lzw = evidence_prob_lzw(self._data)
 
    #下面的函数可以直接调用上面类中定义的变量
    def get_label(self,length,sweetness,color):
        '''获取某一组特征值的类别'''
        self._attrs = [length,sweetness,color]
        res = {}
        for label in self._labels:
            prob = self._priori_prob[label]#取某水果占比率
            #print("各个水果的占比率：",prob)
            test_char=str(f"P({label})")
            for attr in self._attrs:
                #单个水果的某个特征概率除以总的某个特征概率 再乘以某水果占比率
                prob*=self._likelihold_prob[label][attr]/self._evidence_prob[attr]
                test_char+=str(f"*P({attr}|{label})/P({attr})")
            #print(prob)
            res[label] = prob
          #  print (test_char)
 
        #print(res)
        return res

    def get_label_lzw(self,length,sweetness,color):
        '''获取某一组特征值的类别'''
        self._attrs = [length,sweetness,color]
        res = {}
        for label in self._labels:
            prob = self._priori_prob[label]#取某水果占比率
            #print("各个水果的占比率：",prob)
            test_char=str(f"P({label})")
            for attr in self._attrs:
                #单个水果的某个特征概率除以总的某个特征概率 再乘以某水果占比率
                prob*=self._likelihold_prob[label][attr]
                test_char+=str(f"*P({attr}|{label})")
            #print(prob)
            res[label] = prob
         #   print (test_char)
 
        #print(res)
        return res


########生成测试数据
def random_attr(pair):
    """
    function:生成0-1之间的随机数
    """
    return pair[random.randint(0,1)]
 
def getTestData():
    """
    function:生成测试数据集
    """
    sets = [('long','not_long'),('sweet','not_sweet'),('yellow','not_yellow')]
    test_datasets = []
    for a in sets[0]:
        for b in sets[1]:
            for c in sets[2]:
                test_datasets.append([a,b,c])
    return test_datasets

def main():
    test_datasets = getTestData()
 #   test_datasets=[['not_long','not_sweet','yellow']]
    classfier = navie_bayes_classifier()
    for data in test_datasets:
        print(f"特征值：\t{data}")

        res=classfier.get_label(*data)#*表示多参传入
        print(f"预测结果：\t{res}")#预测属于哪种水果的概率
        #对后验概率排序，输出概率最大的标签
        fruit_type=sorted(res.items(),key=lambda x:x[1],reverse=True)[0][0]
        print('水果类别：',end='\t')
        print (fruit_type)

        res=classfier.get_label_lzw(*data)#*表示多参传入
        print(f"预测结果：\t{res}")#预测属于哪种水果的概率
        fruit_type=sorted(res.items(),key=lambda x:x[1],reverse=True)[0][0]
        print('水果类别：',end='\t')
        print (fruit_type)

 
if __name__ == '__main__':
    main()
