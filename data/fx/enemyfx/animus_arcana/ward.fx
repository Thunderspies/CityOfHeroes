###########################################################

FxInfo

	Flags DontSuppress

###########################################################

Condition
		On Time
		Time 0

	Event
		Ename	RuneAnchor
		Type	Local #Local
		At	Head
		bhvr behaviors/genericparticlefade.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			PyrRotate 	-90 180 0
		End
		Sound	Sewer_RuneFireEmblem_Loop 50 50 .15
	End
End


Condition
		On Time
		Time 0

	Event
		Ename	Rune
		Type	Local
		At	RuneAnchor
		BhvrOverride
			Spin	0 0.003 0
		End
		part	:N_FireRune.part
		part	:N_FireRune_OuterGlow.part
	End

End

Condition
		On Time
		Time 0

	Event
		EName	Sigil
		Type	Local
		At	Head
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			PositionOffset	 0 4 0
			Alpha		255
			StartColor 	255 168 0
			Scale 		15 15 15
			PyrRotate 	-90 180 0
			Spin	0 0.03 0
		End
		Geom	FX_Sigil_Base
		Sound TorturedWind_Loop 500 500 0.75
		#Lifespan 20
	End

	Event
		EName	Rays
		Type	Local
		At	Head
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			PositionOffset	 0 4 0
			Alpha		64
			StartColor 	255 168 0
			Scale 		15 15 15
			PyrRotate 	-90 180 0
			Spin	0 0.03 0
		End
		Geom	FX_SigilRays
		#Lifespan 20
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	bubbles
#		Type	Local 
#		At	Head
#		Geom	FX_RuneGlows
#		bhvr	behaviors/Rune_spin.bhvr
#		BhvrOverride
#			Spin	0 0.03 0
#		End
#	End
#
#	Event
#		EName 	OrbSpot1
#		Type	Local 
#		At	bubbles
#		  AltPiv 1
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#	Event
#		EName 	OrbSpot2
#		Type	Local 
#		At	bubbles
#		  AltPiv 2
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part	
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#	Event
#		EName 	OrbSpot3
#		Type	Local 
#		At	bubbles
#		  AltPiv 3
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#	Event
#		EName 	OrbSpot4
#		Type	Local 
#		At	bubbles
#		  AltPiv 4
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#
#	Event
#		EName 	OrbSpot5
#		Type	Local 
#		At	bubbles
#		  AltPiv 5
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#
#
#	Event
#		EName 	OrbSpot6
#		Type	Local 
#		At	bubbles
#		  AltPiv 6
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#
#	Event
#		EName 	OrbSpot7
#		Type	Local 
#		At	bubbles
#		  AltPiv 7
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#
#	Event
#		EName 	OrbSpot8
#		Type	Local 
#		At	bubbles
#		  AltPiv 8
#		part1	World/Lairs/COT/GlowOrb.part
#		Part2	World/Lairs/COT/GlowOrbStar.part
#		part3	World/Lairs/COT/GlowOrb2.part
#		part4	World/Lairs/COT/GlowOrbTrail.part
#	End
#
#
#End

###########################################################

End
