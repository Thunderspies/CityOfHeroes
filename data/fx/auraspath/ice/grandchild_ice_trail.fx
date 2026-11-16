#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time
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
		Part	:Frost01.part
		Part	:Frost02.part
	End


End

#########################################################

End