#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### HEAD GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Eyes
		part	:FireEyesLeft01_small.part
		part	:FireEyesLeft01_small.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Eyes
		part	:FireEyesRight01_small.part
		part	:FireEyesRight01_small.part
		Bhvr	:EyeNodeR.bhvr
	End

End

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
	End

	Event	
		ENAME	Fire1
		Type	Local
		At	HeadNode
		part	:FireFlame01_small.part
		part	:FireEmbers01_small.part
		##part	:FireCrawl01.part
		part	:FireBase01_small.part
		Pmagnet Head
	End

End


End		