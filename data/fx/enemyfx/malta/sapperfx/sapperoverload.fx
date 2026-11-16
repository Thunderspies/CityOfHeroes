#############################################################
## SapperOverload.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound overload3 110 110 1.0
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound overload1 110 110 .5
	End
End


Condition
	On 	Time
	Time 	31

	Event
		EName 	GunTip
		Type	local
		At	WepR
		Bhvr	:SapperGunTipShoot.bhvr
		Lifespan 5
	End

	Event
		Type	Local
		At	GunTip
		Part	:SapperGunTipAttackGlow.Part
		POther	Target
		Lifespan 5
	End

	Event
		Type	Start
		At	GunTip
		Part	:SapperStream01.part
		Part	:SapperStream02.part
		Part	:SapperStream03.part
		Part	:SapperStream04.part
		POther	Target
		Lifespan 5
	End

	Event
		Type	Local
		At	Target
		Part	:SapperGunTipAttackGlow.Part
		Lifespan 5
	End

	Event
		Type	Local
		At	Target
		Part	:SapperGunTipTargetGlow.Part
		Lifespan 5
	End
End

#########################################################
####################     BLAST    #######################
#########################################################

Condition
	On	Time
	Time	36

	Event
		ENAME	ChestGlow
		Type	Start
		At	T_Hips
		part	:SapperOverloadPop1.part
		part	:SapperOverloadHitBolts1.part
		part	:SapperOverloadHitBoltGlows1.part
		part	:SapperOverloadHitBoltGlows2.part
		PMagnet	T_Hips
		Lifespan 25
	End
End

#############################################################

End