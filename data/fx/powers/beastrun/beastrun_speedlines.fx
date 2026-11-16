#############################################################
## Hop_Activate.fx
#############################################################

FxInfo


#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		#Sound BeastRun_Activate_01 50 50 0.5
	End

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

