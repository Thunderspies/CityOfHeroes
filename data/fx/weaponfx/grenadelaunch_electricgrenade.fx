#############################################################
## ElectricityBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	weporigin
		Type	local
		At	WepR
		Geom	FlameThrower
	End

	Event
		EName 	Blast
		Type	Start
		At	weporigin
		Altpiv	1
		Geom	Fx_GunBurst
		Sound Grenade2 120.0 120.0 1.0
		Lifespan 3
	End

	Event
		EName 	tracer
		Type	start
		At	weporigin
		Altpiv	1
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	wepR
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Magnet	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	Local
		At	T_Root
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchs3.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchs2.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchs1.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchs.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchsSpinner.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningRing2.part
		Part	POWERS\ItemsAndTempPowers\ElectricityGrenade\ChainLightningArchsInverted.part
		Sound Grexplo1 70 70 .77
		lifespan 10
	End

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End