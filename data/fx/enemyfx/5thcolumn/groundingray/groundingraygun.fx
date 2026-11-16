#############################################################
## GroundingRayGun.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName 	GunTip
		Type	local
		At	WepR
		Bhvr	:GroundingRayGunTip.bhvr
	End

	Event
		EName 	GunBack
		Type	local
		At	WepR
		Bhvr	:GroundingRayGunTip.bhvr
		BhvrOverride
			PositionOffset	1.5 0 .33
		End
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName 	Gun
		Type	Local
		At	WepR
		Geom	Soul_Tickler
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 0.6
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	GunBackFX
		Type	Local
		At	GunBack
		Part	:ElectricBolts.part
		Part	:Beam_Small.part
		Part	:BeamCore_Small.part
		Part	:SapperStream_Small.part
		POther 	GunTip
		PMagnet GunTip
		Sound GroundingRayIdle_Loop 40 40 0.1
	End

	Event
		EName 	GunTipFX
		Type	local
		At	GunTip
		Part	:ElectricSparks.part
		Part	:Embers2.part
		Part	:SapperGunTipGlow1.Part
		Sound GroundingRayArm 40 40 0.75
	End
End

#############################################################

End