#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset 0 0.4 0
		End
		Part	:Toxic_Continuing.part
		Part	:Toxic_Continuing_Add.part
	End

End

#############################################################

End