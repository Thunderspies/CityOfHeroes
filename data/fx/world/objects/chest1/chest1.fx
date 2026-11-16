#########################################################
##	template

FxInfo
#Lifespan 10


#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On	Time
	Time	0 

	Event	
		Type	Local
		At	Origin
		part	:ChestRays01.part
		part	:ChestRays02.part
		bhvr	:ChestOffset02.bhvr	
	End

	Event	
		Type	Local
		At	Origin
		part	:ChestFloorGlow01.part
		part	:ChestFloorRays01.part
		bhvr	:ChestFloorOffset01.bhvr	
	End

	Event	
		Type	Local
		At	Origin
		part	:ChestGlow01.part
		bhvr	:ChestOffset01.bhvr	
	End


End


End		