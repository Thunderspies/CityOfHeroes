#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Lifespan	55


#########################################################
####################### AUDIO ###########################
#########################################################
Condition

	On 	Time
	Time	0
		
	Event 	Type 	Local
		At 	origin
		Sound Darkfear1 70 70 .4

	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	Eyenode
		Type	Local
		At	hair
		Geom	HairToEyesAdjustment
	End

	Event	
		ENAME	Headnode
		Type	Local
		At	hair
		Geom	HairToHeadAdjustment
	End


End

#########################################################
####################    BURST    ########################
#########################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	AfraidSkull01
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidSkull02.part
		Bhvr	Behaviors/GenericparticleFade.bhvr
		part	:Glow01.part
		Lifespan 45
	
	End
	Event	
		ENAME	AfraidSkull02
		Type	Local
		At	Head
		part	:AfraidSpots02.part
		Lifespan 35
	
	End
End

End	


