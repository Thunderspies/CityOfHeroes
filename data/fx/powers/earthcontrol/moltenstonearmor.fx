#############################################################
## MoltenStoneArmor.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound MoltenStone 100 100 0.96
	End
End

Condition
	On	Time
	Time 	5

	Event
		Type	Local
		At 	origin
		Sound Molten_loop 100 100 0.55
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	FootLa
		Type	Local
		At	FootL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_FootL
	End


	Event
		Ename	FootRa
		Type	local
		At	FootR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_FootR
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_LLegL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_LLegR
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_ULegL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_ULegR
	End

##	Event
##		Type	Local
##		At	HandL
##		Bhvr 	behaviors\StoneArmorHand2.bhvr
##		geom	FX_MagmaArmor_HandL
##	End
##
##	Event
##		Type	Local
##		At	HandR
##		Bhvr 	behaviors\StoneArmorHandL2.bhvr
##		geom	FX_MagmaArmor_HandR
##	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_LArmL
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_LArmR
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_Chest
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_UArmL
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	FX_MagmaArmor_UArmR
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Ename	firefeet
		Type	Local
		At	FootLa
		altpiv	1
		Part	:footFlame.part
		Part	:footspark.part
	End

	Event
		Ename	firefeet2
		Type	Local
		At	FootRa
		altpiv	1
		Bhvr	:OffsetLavaFoot.bhvr
		Part	:footFlame.part
		Part	:footspark.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UARML
		Part	:LavaFlamesShoulders.part
		Part	:LavaFlamesShoulders1.part
	End

	Event
		Type	Local
		At	UARMR
		Part	:LavaFlamesShoulders.part
		Part	:LavaFlamesShoulders1.part
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Type	Local
		At	LARML
		Part	:LavaSteam.part
	End

	Event
		Type	Local
		At	LARMR
		Part	:LavaSteam.part
	End

	Event
		Type	Local
		At	LlegL
		Part	:LavaSteam.part
	End

	Event
		Type	Local
		At	LlegR
		Part	:LavaSteam.part
	End

	Event
		Type	Local
		At	Head
		Part	:LavaSteamTrails.part
	End

	Event
		Type	Local
		At	Hips
		Part	:LavaSteamTrails.part
	End

	Event
		Type	Local
		At	UlegL
		Part	:LavaFlamesHips.part
		Part	:LavaFlameHips1.part
	End

	Event
		Type	Local
		At	UlegR
		Part	:LavaFlamesHips.part
		Part	:LavaFlameHips1.part
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	20
	Chance	0.13

	Event
		Type	Local
		At	UARMR
		Part	:LavaSpray.part
		Lifespan 20
	End
End

Condition
	On 	Cycle
	Time 	20
	Chance	0.13

	Event
		Type	Local
		At	UARML
		Part	:LavaSpray.part
		Lifespan 20
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.4

	Event
		Type	Local
		At	Hips
		Part	:LavaSpray2.part
		Lifespan 20
	End
End

#############################################################

Condition
	On 	death

	Event
		Type	start
		At	hips
		Part	:StoneArmordirtExplosion2.part
		Part	:StoneArmordirtExplosionFlame.part
		Lifespan 17
	End

	Event
		Type	start
		At	hips
		Part	:StoneArmorSparks.part
		Lifespan 17
	End

	Event
		Type	start
		At	Root
		Part	:StoneArmorSparks.part
		Lifespan 17
	End
End

#############################################################

End