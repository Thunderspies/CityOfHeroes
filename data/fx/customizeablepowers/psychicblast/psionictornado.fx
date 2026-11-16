#############################################################
## PsionicTornado.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################


Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime
		Type	start
		At	Root
		bhvr	CustomizeablePowers\PsychicBlast\slowpsonictornadomotion.bhvr
		BhvrOverride
			TrackRate		1.7
		End
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName 	Ring1
		Type	posit
		At	Prime
		bhvr	CustomizeablePowers\PsychicBlast\Wirlwind.bhvr
		Part	CustomizeablePowers\PsychicBlast\Ensnare.part
		Part	CustomizeablePowers\PsychicBlast\Ensnare2.part
		Part	CustomizeablePowers\PsychicBlast\EnsnareFlat.part
		Part	CustomizeablePowers\PsychicBlast\EnsnareMist2.part
		Sound Whirlwind_loop 80 80 .65
	End

	Event
		EName 	Ring2
		Type	posit
		At	Prime
		bhvr	CustomizeablePowers\PsychicBlast\Wirlwind1.bhvr
		Part2	CustomizeablePowers\PsychicBlast\TornadoCoreRings.part
		Part	CustomizeablePowers\PsychicBlast\TornadoRings.part
		Part	CustomizeablePowers\PsychicBlast\Leaves.part
		Sound psionic4 80 80 .65
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	13

	Event
		EName 	Rings
		Type	posit
		At	Ring1
		Bhvr	CustomizeablePowers\PsychicBlast\PsionicTornado.bhvr
		BhvrOverride
			Alpha		150
			TintGeom	1
		End
		geom	Tintable_FX_AirWisp01
		LifeSpan 14
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	burst
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\PsychicBlast\Infernobubble2.bhvr
		Part1	CustomizeablePowers\PsychicBlast\PitBust1.part
		Part2	CustomizeablePowers\PsychicBlast\PitBust2.part
		Part3	CustomizeablePowers\PsychicBlast\PitBustRings.part
		Part4	CustomizeablePowers\PsychicBlast\PitBustBits.part
		Part5	CustomizeablePowers\PsychicBlast\flash.part
	End
End

#############################################################

End