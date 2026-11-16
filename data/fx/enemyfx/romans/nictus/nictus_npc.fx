#############################################################
## HEADER
#############################################################

FxInfo

FLAGS	InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound nictuscrystal_loop 80 80 .91
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Offset
		Type	Local
		At	Hips
		Bhvr	ENEMYFX\Romans\Nictus\Nictus.bhvr
	End

	Event
		Type	Local
		At	Offset
		Part	:Tendrils_Black.part
		Part	:Tendrils_Blue.part
		Part	:Tendrils_Purple.part

		part	:Energy_Blue.part
		part	:Energy_Purple.part
		part	:Energy_Black.part

		#part	:Core.part
		#part	:CoreBlue.part
		#part	:CorePurple.part

		Part	:Stars.part
		Part	:Stars_Dark.part
	End
End

#############################################################

End