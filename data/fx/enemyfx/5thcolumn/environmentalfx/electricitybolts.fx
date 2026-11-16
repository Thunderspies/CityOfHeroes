#############################################################
## Lightning_Red.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	nodeN
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0 20 0
		End
		Geom	centerdummy
	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	WepR
		altpiv	1
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	nodeN

	End

End

##############################################################
#
#End