#############################################################
## SmokeBomb.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

#############################################################

	Event
		Type	Local
		At	LArmL
		Part	:SpeedLines.part
	End

	Event
		Type	Local
		At	LArmR
		Part	:SpeedLines.part
	End

	Event
		Type	Local
		At	LLegL
		Part	:SpeedLines_Small.part
	End

	Event
		Type	Local
		At	LLegR
		Part	:SpeedLines_Small.part
	End
End



#############################################################

