#############################################################
## Gunfire_GernadeLauncherNoHit.fx
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
		Type	Start
		At	weporigin
		Altpiv	1
		Geom	Fx_GunBurst
		Lifespan 2
	End

	Event
		Type	start
		At	weporigin
		Altpiv	1
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Sound Grenade2 120.0 120.0 1.0
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	weporigin
		Altpiv	1
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_chest
		LookAt	T_chest
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End