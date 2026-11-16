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
		bhvr	:slowpsonictornadomotion.bhvr
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
		bhvr	behaviors\Wirlwind.bhvr
		Part	:Ensnare.part
		Part	:Ensnare2.part
		Part	:EnsnareMist.part
		Part	:Leaves.part
		Part	:EnsnareMist2.part
		Part	:EnsnareFlat.part
		Part	:TornadoRings.part
		Sound Whirlwind_loop 80 80 .65
	End

	Event
		EName 	Ring2
		Type	posit
		At	Prime
		bhvr	behaviors\Wirlwind1.bhvr
		Part2	:TornadoCoreRings.part
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
		Bhvr 	behaviors\PsionicTornado.bhvr
		geom	PsionicWisp	#FX_AirWisp01
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
		bhvr    behaviors\powers\firecontrol\Infernobubble2.bhvr
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Part3	:PitBustRings.part
		Part4	:PitBustBits.part
		Part5	:flash.part
	End
End

#############################################################

End