#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/NatterlingProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalTrail.part
		Part	:CrystalTrail2.part
		Part	:FleshTrail.part
		Part	:FleshTrail2.part
		Part	:FleshTrailBlack.part
		Geom	NatterlingProjectile
		Sound projectilewhoosh2 80 80 .55
	End

	Event
		EName 	Prime1
		Type	Start 
		At	WepR
		Bhvr	behaviors/NatterlingProjectile.bhvr
		Magnet	Target
		LookAt	Target

		Geom	NatterlingProjectileCrystals
		
	End

End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target


	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime1
		Type	Destroy
	End

End

End	


			