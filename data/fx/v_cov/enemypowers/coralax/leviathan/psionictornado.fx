#########################################################
##	FireRing
##
FxInfo

LifeSpan	200

Condition
	On 	time
	Time 	28
	Event
		Ename  Chargeup
		Type	local 
		At	wepr
#	geom    Testing_Target

	Bhvr	:ROTfwd.bhvr
	End



	Event
		EName 	Rings
		Type	posit 
		At	Chargeup
	
		Part	POWERS\Psionics\Ensnare.part
		Part	POWERS\Psionics\Ensnare2.part
		Part	POWERS\Psionics\EnsnareMist.part
		Part	POWERS\Psionics\Leaves.part
		Part	POWERS\Psionics\EnsnareMist2.part
		Part	POWERS\Psionics\EnsnareFlat.part
		Part	POWERS\Psionics\TornadoRings.part
		
		Part2	POWERS\Psionics\TornadoCoreRings.part
		bhvr	behaviors/Wirlwind.bhvr	
		lifespan 35
	End




end

Condition
	On 	cycle
	Time 	13

	Event
		EName 	Rings
		Type	posit 
		At	Ring1
	
		Bhvr 	behaviors/PsionicTornado.bhvr
		geom	PsionicWisp	#FX_AirWisp01
		LifeSpan	14
	End

	



End
	
Condition
	On 	Time
	Time 	50

	Event
		EName	Chargeup
		Type	Destroy
	End

	Event
		EName 	Prime
		Type	start 
		At	wepr
		#bhvr	behaviors/slowpsonictornadomotion.bhvr
		bhvr	POWERS\Psionics\slowpsonictornadomotion.bhvr
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName 	Ring1
		Type	posit 
		At	Prime
	
		Part	POWERS\Psionics\Ensnare.part
		Part	POWERS\Psionics\Ensnare2.part
		Part	POWERS\Psionics\EnsnareMist.part
		Part	POWERS\Psionics\Leaves.part
		Part	POWERS\Psionics\EnsnareMist2.part
		Part	POWERS\Psionics\EnsnareFlat.part
		Part	POWERS\Psionics\TornadoRings.part
		bhvr	behaviors/Wirlwind.bhvr	
		Sound Whirlwind_loop 80 80 .65
	End

	Event
		EName 	Ring2
		Type	posit 
		At	Prime
	
		
		Part2	POWERS\Psionics\TornadoCoreRings.part
		#Part3	POWERS\Psionics\TornadoRings.part
		bhvr	behaviors/Wirlwind1.bhvr	
		Sound psionic4 80 80 .65
		
				
	End
End



Condition
	On 	PrimeHit
	
	Event
		EName 	burst
		Type	start 
		At	T_Root
		bhvr    behaviors/powers/firecontrol/Infernobubble2.bhvr
		Part1	POWERS\Psionics\PitBust1.part
		Part2	POWERS\Psionics\PitBust2.part
		Part3	POWERS\Psionics\PitBustRings.part
		Part4	POWERS\Psionics\PitBustBits.part
		Part5	POWERS\Psionics\flash.part
	End
End

End
################################################################
		