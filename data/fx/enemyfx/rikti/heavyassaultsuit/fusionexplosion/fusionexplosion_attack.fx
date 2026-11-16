#########################################################
## Rikti Heavy Assault Suit - Fusion Explosion	
#########################################################

#########################################################
## WepR on frames 3 and 19
## WepR on frames 11 and 27
#########################################################

FxInfo

## PRE-FIRE #######################################################

Condition
	On 	Time
	Time 	45

	Event
		Ename	Flash
		Type	local
		At	WepR
		Part1	WEAPONFX\BurstStreaks.part
		Part2	WEAPONFX\BurstEmbers.part
	End

	Event
		Ename	Flash
		Type	local
		At	WepR
		Part1	WEAPONFX\RiktiMachineGunGlowStart1.part
		Part2	WEAPONFX\RiktiMachineGunGlowStart.part
		Lifespan	5
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Ename	Flash
		Type	local
		At	WepL
		Part1	WEAPONFX\BurstStreaks.part
		Part2	WEAPONFX\BurstEmbers.part
	End

	Event
		Ename	Flash
		Type	local
		At	WepL
		Part1	WEAPONFX\RiktiMachineGunGlowStart1.part
		Part2	WEAPONFX\RiktiMachineGunGlowStart.part
		Lifespan	5
	End
End

## PROJECTILES #######################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Geom	LightDummy2
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	WEAPONFX\RiktiBlasterBullet.part
		Part2	WEAPONFX\RiktiMachineGunDarkEnergy.part
		Part3	WEAPONFX\RiktiMachineGunBlueEnergy.part
		Part4	WEAPONFX\RiktiBlasterBullet.part
		Magnet	Target
		LookAt	Target
		Sound RiktiAssault2 100.0 100.0 .6
	End

	Event
		EName 	PrimeA
		Type	Start 
		At	Prime
		ChildFX	"ENEMYFX\Rikti\HeavyAssaultSuit\FusionPulse\Fusion_Pulse_Projectile.fx"
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	Start 
		At	WepL
		Geom	LightDummy2
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	WEAPONFX\RiktiBlasterBullet.part
		Part2	WEAPONFX\RiktiMachineGunDarkEnergy.part
		Part3	WEAPONFX\RiktiMachineGunBlueEnergy.part
		Part4	WEAPONFX\RiktiBlasterBullet.part
		Magnet	Target
		LookAt	Target
		Sound RiktiAssault2 100.0 100.0 .6
	End

	Event
		EName 	Prime2A
		Type	Start 
		At	Prime
		ChildFX	"ENEMYFX\Rikti\HeavyAssaultSuit\FusionPulse\Fusion_Pulse_Projectile.fx"
	End
End


#########################################################

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	PrimeA
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit
	
	Event
		Ename 	Prime2
		Type	Destroy
	End

	Event
		Ename 	Prime2A
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	120

	Event
		Ename 	ALL
		Type	Destroy
	End
End

#########################################################

End