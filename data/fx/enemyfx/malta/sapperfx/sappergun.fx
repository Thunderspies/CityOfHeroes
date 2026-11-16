#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Gun
		Type	Local
		At	WepR
		Geom	GEO_WepR_Malta_Sapper
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .6
	End

	Event
		EName 	GunTip
		Type	local
		At	WepR
		Bhvr	:SapperGunTipOffset.bhvr
		Sound sapper2_loop 35 35 .2
	End

	Event
		EName 	GunBack
		Type	local
		At	WepR
		Bhvr	:SapperGunBackOffset.bhvr
		Sound sapper_loop 35 35 .19
	End

	Event
		EName 	GunExhaust
		Type	local
		At	WepR
		Bhvr	:SapperGunExhaustOffset.bhvr
	End

	Event
		EName 	GunExhaust2
		Type	local
		At	WepR
		Bhvr	:SapperGunExhaustOffset2.bhvr
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	GunBackFX
		Type	local
		At	GunBack
		#Part	:SapperGunFX1.Part
	End

	Event
		EName 	GunTipFX
		Type	local
		At	GunTip
		Part	:SapperGunTipGlow.Part
		Part	:SapperGunTipPop2.Part
		Part	:SapperGunTipPop1.Part
		Part	:SapperGunTipGlow1.Part
		Part	:SapperGunTipGlow2.Part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	GunExhaustFX
		Type	local
		At	GunExhaust
		Part	:SapperGunExGlow1.Part
		Part	:SapperGunExFx2.Part
		Part	:SapperGunExFx1.Part
		#Lifespan 5
	End

	Event
		EName 	GunExhaustFX2
		Type	local
		At	GunExhaust2
		Part	:SapperGunExGlow1.Part
		Part	:SapperGunExFx2.Part
		Part	:SapperGunExFx1.Part
		#Lifespan 5
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	GunBackFX
		Type	Posit
		At	GunBack
		Part	:SapperGunGlow1.Part
		Part	:SapperGunStream01.part
		Part	:SapperGunFX1.part
		POther GunTip
		PMagnet GunTip
	End

	Event
		EName 	GunBackFX2
		Type	local
		At	GunBack
		Part	:SapperGunTipGlow.Part
		Part	:SapperGunTipGlow1.Part
		Part	:SapperGunTipGlow2.Part
	End
End

#############################################################

End