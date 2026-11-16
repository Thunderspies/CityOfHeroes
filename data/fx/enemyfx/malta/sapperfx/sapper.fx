#############################################################
## Sapper.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	origin
		Sound Sapper1 90 90 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		EName 	GunTip
		Type	local
		At	WepR
		Bhvr	:SapperGunTipShoot.bhvr
		Lifespan 90
	End

	Event
		Type	Local
		At	GunTip
		Part	:SapperGunTipAttackGlow.Part
		POther	Target
		Lifespan 40 #70
	End

	Event
		Type	Local
		At	GunTip
		Part	:SapperStream01.part
		Part	:SapperStream02.part
		Part	:SapperStream03.part
		Part	:SapperStream04.part
		POther	Target
		Lifespan 30 #60
	End
End

Condition
	On 	Time
	Time 	31

	Event
		Type	Local
		At	Target
		Part	:SapperGunTipAttackGlow.Part
		Lifespan 40 #70
	End
End

#############################################################

End