#############################################################
## Gunfire_ColdEnergyShotgun.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	origin
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	LaserAssault
		Bhvroverride
			PositionOffset	-.3 0 0
		End
	End

	Event
		EName 	MussleFlashRigging
		Type	local
		At	Blast
		Altpiv	1
		Bhvroverride
			PositionOffset	-.6 0 0
		End
		Geom	PsionicRigging
	End

	Event
		EName 	MussleFlash
		Type	start
		At	MussleFlashRigging
		altpiv	3
		Bhvroverride
			PyrRotate	80 20 90
		End
		Part	:SonicCoreElipse.part
		Part	:SonicElipse.part
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	Start
		At	Blast
		Altpiv	1
		Bhvr	Behaviors\ShotGun.bhvr
		Bhvroverride
			StartColor	0 0 255
		End
		Geom	Fx_GunBurst
		LifeSpan 2
	End

	Event
		Type	start
		At	Blast
		Altpiv	1
		Part1	:IceBoltHandFlash.part
		Part2	:IceBoltHandFlash2.part
		Part1	:IceBoltSnowSkatter.part
		Part	:IceBoltenergyBallSkatter.part
		Sound coldshotgun 90 90 .9
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
		At	Blast
		Altpiv	1
		Bhvr	behaviors\powers\coldcontrol\icebolt.bhvr
		Bhvroverride
			TrackRate	5
			Alpha		155
		End
		Geom	FX_IceBlast
		Part2	:EnergyShotgunTailThick.part
		Part2	:EnergyShotgunTail.part
		Part2	:EnergyShotgunTail.part
		Part3	:IceBoltSnowtrail.part
		Part	:PowerBalltrail.part
		Part4	POWERS\ColdControl\IceBoltMistTrail.part
		Part5	:IceBoltIceDebris2.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End