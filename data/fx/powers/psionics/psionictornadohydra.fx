#########################################################
##	FireRing
##
FxInfo

LifeSpan	200




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
		EName 	Prime
		Type	start 
		At	Root
		bhvr	behaviors/slowpsonictornadomotion.bhvr
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		EName 	Ring1
		Type	posit 
		At	Prime
	
		Part	:Ensnare.part
		Part	:Ensnare2.part
		Part	:EnsnareMist.part
		Part	:Leaves.part
		Part	:EnsnareMist2.part
		Part	:EnsnareFlat.part
		Part	:TornadoRings.part
		bhvr	behaviors/Wirlwind.bhvr	
		Sound Whirlwind_loop 80 80 .65
	End

	Event
		EName 	Ring2
		Type	posit 
		At	Prime
	
		
		Part2	:TornadoCoreRings.part
		#Part3	:TornadoRings.part
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
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Part3	:PitBustRings.part
		Part4	:PitBustBits.part
		Part5	:flash.part
	End
End

End
################################################################
		