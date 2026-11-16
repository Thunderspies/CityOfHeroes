#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time #Cycle
	Time		0
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Frost01_MOVEMENT.part
		Part	:Frost02_MOVEMENT.part
		Part	:Mist.part
		Part	:Sparkles.part
	End


End

#########################################################

End