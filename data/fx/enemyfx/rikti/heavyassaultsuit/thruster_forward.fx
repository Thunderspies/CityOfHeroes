#############################################################
## Rikti Heavy Assault Suit - Thruster FX
#############################################################

FxInfo

Flags InheritAlpha

## SOUND FX ###########################################################

#Condition
#	On	Time
#	Time	0
#	
#	Event	
#		Ename   targetchill
#		Type	Local
#		At	Chest
#		Sound	flight2_loop 40.0 40.0 .09
#	End
#End

## PERSISTENT FX ###########################################################

Condition
	On	Time
	Time	0
	
	Event	
		Ename   PodThruster
		Type	Local
		At	T1_L
		#part2	:ThrusterTrail_Alpha.part
		part2	:ThrusterTrail_Bright.part
		#part2	:ThrusterTrail_Flames.part
		part2	:Thruster_Flare.part
	End

	Event	
		Ename   PodThruster
		Type	Local
		At	T1_R
		#part2	:ThrusterTrail_Alpha.part
		part2	:ThrusterTrail_Bright.part
		#part2	:ThrusterTrail_Flames.part
		part2	:Thruster_Flare.part
	End
End

#############################################################

End				