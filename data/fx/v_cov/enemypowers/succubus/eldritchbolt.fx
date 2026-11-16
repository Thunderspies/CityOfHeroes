#########################################################
##	Fire_Ball
##
FxInfo


LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:BaleFireHands.part
		Part4	:DarkSparks.part
		#Sound	Fireball1 100.0 30.0 .66
	
	End


End

Condition
	On 	Time
	Time  	8
	Event
		Type	Local
		At	Origin
		Sound Forcebolt 80.0 80.0 .8
	End

End


Condition
	On 	Time
	Time 	5
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:BaleFireFlash.part
		#Sound	Fireblastfly 80 30 .9
	
	End

End

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound Fireblastfly 80 80 .9
	End

End

Condition
	On 	Time
	Time 	16

	Event
		EName	SwordFlash2
		Type	Destroy
		
	
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:BaleFireHands2.part
		Part2	:HellBoltTail2.part
		Part3	:HellBoltTail.part
		Part4	:DarkSparkTrail.part
		Part5	:BaleFireHands2.part
	End


End

Condition
	On	PrimeHit
				
	Event
		Ename	FireBallHit
		Type	local
		At	target
		Part	:HellBoltFlash.part
		Part	:HellBoltFlash.part
		Part	:BaleFireExplosion.part
		Part	:HellBoltHitSparks.part
		ChildFX	:EldritchExplosion.fx
	End
		
	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On	Time
	Time	100
			
	Event
		Ename	all
		Type	Destroy
	End

End

End	


			