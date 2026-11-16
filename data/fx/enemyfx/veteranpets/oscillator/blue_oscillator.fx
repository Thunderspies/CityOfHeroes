#############################################################
## Veteran Reward Combat Pet - Blue Clockwork Oscillator
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	chest
		Geom	proplink
	End

	Event
		Type	Local
		At	Prime
		Altpiv	1
		Part1	:Blue_Smoke_01.part
		Part1	:Blue_Smoke_02.part
		#Sound	heli_loop 25 20 .22
	End

	Event
		Type	Local
		At	Prime
		Altpiv	1
		Part4	:Blue_Glow_01.part
		Part4	:Blue_Core_01.part
		Part4	:Blue_Core_01.part
		Part3	:Blue_Sparks_01.part
	End

	Event
		EName 	link2
		Type	Local
		At	Prime
		AltPiv	1
		Bhvr	behaviors\SprocketPropeller_spin.bhvr
		Geom	Propeller2
	End
End

#############################################################

Condition
	On 		triggerbits
	Triggerbits	DEATH

	Event
		EName 	ALL
		Type	Destroy
	End
End

#############################################################

End