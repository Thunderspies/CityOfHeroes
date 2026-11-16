#############################################################
## Child_FireShield_Flames.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Offset1
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.1 -1.6255
		End
	End

	Event
		EName 	Offset2
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.0 0.875
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Offset1
		Part	:MagmaShield_Flames1.part
		Part	:MagmaShield_Sparks1.part
		POther	Offset2
	End
End

#############################################################

End