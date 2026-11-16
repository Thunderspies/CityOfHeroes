#########################################################
##	Recluse Ranged Cone Attack
##############################################################################################
##############################################################################################
######   Defs: BC = Energy Ball Core; BE = Energy Ball Core Extent Targets
##############################################################################################
##############################################################################################

FxInfo

LifeSpan	90

##############################################################################################
######  build up stretch from hand to elbowt    LOWER ARMs
##############################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	LineGoal
		Type	Local
		At	Origin
		bhvrOverride
			PositionOffset	-1 0 0
		End

#		Geom	Testing_TargetB
	End
End


Condition
	On Time 
	Time 3
	
	Event
		EName	ForeArm_LineL
		Type 	Local
		At	Origin
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineHuman.part
		part2	:P_GlowSparks_RecluseLineHuman.part
		part3	:P_GlowCore_RecluseLineHuman.part
#		Geom	Testing_TargetB
		Lifespan  75
		POther	LineGoal
	End
End

End

























