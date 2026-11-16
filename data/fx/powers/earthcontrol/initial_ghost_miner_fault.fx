#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	origin
End

Input  
	Inpname	target
End

Input  
	InpName	Root
End

Input  
	InpName	T_Root
End

Input  
	InpName	Mystic
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	origin
		
		Sound punch13 100.0 100.0 .85
	End

End



Condition
	On 	Time
	Time 	70

	Event
		EName	Quake
		Type	local
		At	root
		
		bhvr	behaviors/SlightQuake.bhvr
				
	End

End

Condition
	On 	Time
	Time 	60

	Event
		EName	cracks
		Type	start
		At	mystic
		Bhvr	Behaviors/CameraShakeFault.bhvr
		Part1	:FaultCrack01.part		
		Part2	:superStrengthDust1.part
		part3	:superStrengthDust2.part
	End

End

Condition
	On 	Time
	Time 	59

	Event
		EName	prime
		Type	start
		At	mystic
		Bhvr	Behaviors/Projectile2.bhvr
		Part1	:FaultCrack.part	
		Part2	:FaultCrack2SmallFaint.part
		Part3	:FaultCrack1SmallFaint.part
		Magnet	T_Root
		Sound quake1 120 120 .9

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
	
	Event
		EName 	hitrings
		Type	start
		At	T_Root

		Part1   :Faultflash.part
		Part2   :Faultflash2.part
		Part3   :FaultCrackHitRadius.part
		Part4   :FaultCrackHitRadius2.part
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault
		bhvr	behaviors/fault.bhvr
		LifeSpan	30
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault01
		bhvr	behaviors/fault1a.bhvr
		LifeSpan	30
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault02
		bhvr	behaviors/fault2.bhvr
		LifeSpan	30
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault03
		bhvr	behaviors/fault1.bhvr
		LifeSpan	30
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault04
		bhvr	behaviors/fault2.bhvr
		LifeSpan	30
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Geom	Fault05
		bhvr	behaviors/fault5.bhvr
		LifeSpan	30
	End

	Event
		EName 	StonecrackGeom
		Type	start
		At	T_root
		Bhvr	behaviors/FaultCrackScale.bhvr
		geom	Crack01
		
		LifeSpan	30
		
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	T_Root
		Part	:FaultDustFlatLater2.part
		Part1	:FaultDustLater.part
		Part2	:FaultDust2Later.part
		Part3	:FaultDustFlatLater.part
		Part4	:FaultRadiusRocks.part
		Part	:FaultDustCenter.part
		Part	:FaultDust2Center.part
		LifeSpan	70
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName	cracks
		Type	Destroy
		
	End
	

End

Condition
	On 	Time
	Time 	200

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

