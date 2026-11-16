#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Part	:Electricity_Bolts_Broken_Stationary.part
		part	:Electricity_Stationary_Bolts.part
		part	:Smoke_Stationary.part
		part	:Electricity_Stationary_Head_Bolts.part
		part	:Electricity_Stationary_Ground_Bolts.part
		part	:Electricity_Stationary_Head_Core.part
		part	:Electricity_Stationary_Head_Core_Up.part
		part	:Rings_Expanding.part
		Part	:Rings_Core.part
	End
End

#########################################################

End