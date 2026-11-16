#########################################################
##	template

FxInfo


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
		ENAME	Altar1
		Type	Local
		At	Origin
		part	:LabBeakerBubbles01.part
		part	:LabBeaker01.part
		part	:LabBeaker02.part
		part	:LabBeaker03.part
		part	:LabBeakerRays01.part
		part	:LabBeakerGlow01.part
		bhvr	:LabOffset01.bhvr
	End

End


End		