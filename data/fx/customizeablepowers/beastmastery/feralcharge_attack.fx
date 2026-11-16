###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_GrowlShort_01 50 50 .45
		Sound Dog_GrowlShort_02 50 50 .45
		Sound Dog_GrowlShort_03 50 50 .45
		Sound Dog_GrowlShort_04 50 50 .45
		Sound Dog_GrowlShort_05 50 50 .45
	End
End


Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	Dog_FeralChargeWhoosh 100 100 .9
	End
End

Condition
	On Time
	Time 17
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 1
		Sound Dog_SnarlTug_01 50 50 .45
		Sound Dog_SnarlTug_02 50 50 .45
		Sound Dog_SnarlTug_03 50 50 .45
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 2.0 -15.0
			PYRRotate	90 0 90
		End
		Part	:Air_Dust_Traveling.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 3
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.5 -15.0
			PYRRotate	0 90 180

		End
		Part	:Air_Dust_Traveling_Long.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 1
	End
End
#
#Condition
#	On	Time
#	Time	27
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 0.0 15.0
#		End
#		Part	:Ground_Dust_Traveling.part
#		Lifespan 7
#	End
#End

###########################################################

End