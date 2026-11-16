#########################################################
## Rikti Heavy Assault Suit - Fusion Pulse	
#########################################################

#########################################################
## WepR on frames 3 and 19
## WepR on frames 11 and 27
#########################################################

FxInfo

## FIRE #######################################################

Condition
	On 	Time
	Time 	3

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
	Time 	11

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

Condition
	On 	Time
	Time 	19

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
	Time 	27

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

## FIRST PROJECTILE #######################################################

Condition
	On 	Time
	Time 	3

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

## SECOND PROJECTILE #######################################################

Condition
	On 	Time
	Time 	11

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
		At	Prime2
		ChildFX	"ENEMYFX\Rikti\HeavyAssaultSuit\FusionPulse\Fusion_Pulse_Projectile.fx"
	End
End

## THIRD PROJECTILE #######################################################

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime3
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
		EName 	Prime3A
		Type	Start 
		At	Prime3
		ChildFX	"ENEMYFX\Rikti\HeavyAssaultSuit\FusionPulse\Fusion_Pulse_Projectile.fx"
	End
End

## FOURTH PROJECTILE #######################################################

Condition
	On 	Time
	Time 	27

	Event
		EName 	Prime4
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
		EName 	Prime4A
		Type	Start 
		At	Prime4
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
	On 	Prime3Hit
	
	Event
		Ename 	Prime3
		Type	Destroy
	End

	Event
		Ename 	Prime3A
		Type	Destroy
	End
End

Condition
	On 	Prime4Hit
	
	Event
		Ename 	Prime4
		Type	Destroy
	End

	Event
		Ename 	Prime4A
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