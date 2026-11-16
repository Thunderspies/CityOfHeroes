#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 	Time
	Time	0

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.05 -0.5
		End

		part	:Electricity_Head_Ground_MOVEMENT.part

		part	:Electricity_Trail_MOVEMENT.part
		part	:Electricity_Trail_Broken_MOVEMENT.part

		part	:Electricity_Trail_Ground_MOVEMENT.part
		part	:Electricity_Trail_Ground_Bolts_MOVEMENT.part
	End
End



#########################################################

End