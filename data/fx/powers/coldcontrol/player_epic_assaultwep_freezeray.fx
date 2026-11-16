#############################################################
## Header
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	WepR
		geom	LaserRifle
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Start
		At	C_Chest
		Bhvr	behaviors\powers\coldcontrol\iceWeaponblast.bhvr
		Part2	:IceWeaponSnowBlast.part
		Part3	:IceWeaponMussleFlash2.part
		Part4	:IceWeaponMussleFlash.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	mush
		Type	Start
		At	C_Chest
		Part1	:HeavyMussleFlash.part
		Part2	:HeavyMussleFlash2.part
		Part3	:MussleSnowStreaks.part
		Sound laserblast3 100.0 100.0 1.0
		LifeSpan 3
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	Start
		At	C_Chest
		Bhvr	behaviors\powers\coldcontrol\iceWeaponHeavyblast.bhvr
		Geom	FX_IceBlast
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail3.part
		Part5	:IceWeaponBlastHeavyTail.part
		Part	:IceWeaponBlastHeavyTail.part
		Part2	:IceWeaponBlastTailGlow.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime2
		Type	start
		At	C_Chest
		bhvr	behaviors\Creyspin3b.bhvr
		BhvrOverride
			PositionOffset		-0.125 0.0 0.0
		End
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	Prime2
		altpiv	1
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part
	End

	Event
		Type	local
		At	Prime2
		altpiv	2
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime3
		Type	start
		At	C_Chest
		bhvr	behaviors\Creyspin3a.bhvr
		BhvrOverride
			PositionOffset		0.125 0.0 0.0
		End
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	Prime3
		altpiv	1
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part
	End

	Event
		Type	local
		At	Prime3
		altpiv	2
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part
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

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End
End

#############################################################

End