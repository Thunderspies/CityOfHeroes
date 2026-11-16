#############################################################
## IceWeaponBlastRifle.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	WepR
		geom	energyRifle
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Start
		At	Hookup
		Altpiv	1
		Bhvr	behaviors\powers\coldcontrol\iceWeaponblast.bhvr
		Part2	:IceWeaponSnowBlast.part
		Part3	:IceWeaponMussleFlash2.part
		Part4	:IceWeaponMussleFlash.part
		Sound laserblast1 100.0 100.0 .85
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	Start
		At	Hookup
		Altpiv	1
		Bhvr	behaviors\powers\coldcontrol\iceWeaponblast.bhvr
		Geom	FX_IceBlast
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail3.part
		Part5	:IceWeaponBlastTail.part
		Part	:IceWeaponBlastTail.part
		Magnet	Target
		LookAt	Target
	End

	Event

		Type	local
		At	prime
		Magnet	Target
		LookAt	Target
		Part2	:IceWeaponBlastTailGlow.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename   IceShards
		Type	Posit
		At	target
		Part1	:IceBoltMist.part
		Part2	:ChillTouchSnow.part
		Part3   :ChillingTouchFlash.part
		LifeSpan 20
	End
End

#############################################################

End