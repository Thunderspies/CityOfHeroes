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
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Eyes
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
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
		part	:FireFlame01.part
		part	:FireEmbers01.part
		##part	:FireCrawl01.part
		part	:FireBase01.part
		Pmagnet Head
	End

End


End		