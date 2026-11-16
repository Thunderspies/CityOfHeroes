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
			PositionOffset	0.0 2.05 -0.5
		End

		part	:Electricity_Head_Ground_AIR.part

		part	:Electricity_Trail_AIR.part
		part	:Electricity_Trail_Broken_AIR.part

		part	:Electricity_Smoke_Air.part
	#	part	:Electricity_Trail_Ground_Bolts.part
	End
End



#########################################################

End