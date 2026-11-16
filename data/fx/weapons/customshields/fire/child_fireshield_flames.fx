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
			PositionOffset	0.0 0.0 -1
		End
	End

	Event
		EName 	Offset2
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.0 1
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
		Part1	:FireShield_Glow1.part
		Part3	:FireShield_Flames1.part
		Part3	:FireShield_Flames2.part
		Part4	:FireShield_Core1.part
		Part1	:FireShield_Sparks1.part
		POther	Offset2
	End
End

#############################################################

End