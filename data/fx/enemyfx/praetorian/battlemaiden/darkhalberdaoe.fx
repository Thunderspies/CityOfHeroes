#############################################################
## HalberdAOE.fx
#############################################################

FxInfo
Lifespan	500

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hookup
		Type	Start
		At	Root
		Sound BM_SummonSpears_Hit 200 200 .9
	End

	Event
		EName	HalberdAnchor
		Type	Start
		At	Hookup
		Bhvr	Behaviors/FadeIn.bhvr
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
			PyrRotate	0 0 0
			Spin		0.0 0.0 0.25
			InitialVelocity	0 -0.25 0.0
			Gravity		0.04
			FadeInLength	20
		End
		Part	:DarkSword_Flash_Initial.part
		Part	:DarkSword_Flash_Core_Initial.part
		Part	:DarkSword_Ring_Initial.part
		Geom	FX_BattleMaiden_Halberd
		Lifespan	33
	End


	Event
		EName	HalberdAnchor2
		Type	Local
		At	HalberdAnchor
		AltPiv	2
	End

	Event
		EName	HalberdAnchor4
		Type	Local
		At	HalberdAnchor
		AltPiv	4
	End

	Event
		EName 	Node2
		Type	Local
		At	HalberdAnchor
		AltPiv	1
		Part	:DarkHalberd_Glow_Handle.part
		POther 	HalberdAnchor2
	End

	Event
		EName 	Node2
		Type	Local
		At	HalberdAnchor
		AltPiv	3
		Part	:DarkHalberd_Glow_Head.part
		POther 	HalberdAnchor4
	End
End

Condition
	On	Time
	Time	32

	Event
		Type	start
		At	Hookup
		Bhvr	behaviors/CameraShake01.bhvr
		Lifespan	30
	End
End

Condition
	On	Time
	Time	33

	Event
		Type	StartPositOnly
		At	Hookup
		Part	:DarkShockwave.part
		Part	:DarkExplosion_Plasma_Sparks_Lingering.part
		Part	:DarkExplosion_Plasma_Shockwave.part
		Part	:DarkExplosion_Plasma_Glow.part
		Part	:DarkExplosion_Plasma_Glow_Lingering.part
		Lifespan	5
	End

	Event
		Type	Local
		At	Hookup
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		2.25 2.25 2.25
		End
		Part	:DarkDustRing.part
		Part	:DarkDustRing_Highlight.part
		Lifespan 15
	End

	Event
		EName	Sigil
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			StartColor	1 1 1
			SecondaryTint	20
			Scale		50.0 5.0 50.0
			#SplatFlags	DIFFUSE
		End
		Splat	crackedEarth.tga
		Part	:DarkCrackedEarth.part
		Lifespan 450
	End

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -20.0 0.0
		End
		Part	:DarkGeyser_Streaks.part
		Part	:DarkGeyser_Streaks_Thin.part
		Part	:DarkGeyser_Core.part
		Sound BM_SummonSpears_Hit_Loop 100 100 .75
		Lifespan 400
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 -5.0 0.0
		End
		Part	:DarkGeyser_Nanites.part
		Part	:DarkExplosion_Plasma_Sparks2_Lingering.part
		Lifespan 450
	End
End

Condition
	On	Time
	Time	30
	Repeat	6

	Event
		EName	ThunderTracks
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		#ChildFX	:DarkChild_ThunderStompBranches.fx
	End
End

#############################################################


End