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




#########################################################
####################### AUDIO ###########################
#########################################################
Condition

	On 	Time
	Time	0
		
	Event 	Type 	Local
		At 	origin
		Sound Darkfear1 70 70 .3

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
		part	:AfraidSkull01.part
		Bhvr	Behaviors/GenericparticleFade.bhvr
		Lifespan 45
	
	End
End
Condition
	On	Time
	Time	0

	Event	
		ENAME	Rings
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidSpots.part
		Lifespan 25
	
	End



End	

End	


