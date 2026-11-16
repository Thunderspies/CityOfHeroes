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

		part	:Electricity_Head_Ground.part

		part	:Electricity_Trail.part
		part	:Electricity_Trail_Broken.part

		part	:Electricity_Trail_Ground.part
		part	:Electricity_Trail_Ground_Bolts.part
		ChildFX	:CHILD_Electricity_IMPACT.fx
	End
End



#########################################################

End