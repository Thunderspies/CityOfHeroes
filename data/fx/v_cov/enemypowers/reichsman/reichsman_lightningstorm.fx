#############################################################
## Reichsman_LightningStorm.fx
#############################################################

FxInfo

LifeSpan 160

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR
		Sound ReichsmanLightningStorm_01 130 130 .99
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	WepR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:HandBeams.part
		Part5	:HandBeamsSub.part
		LifeSpan 62
	End
End

#############################################################

Condition
	On 	Time
	Time 	82	#80

	Event
		Ename	Lightning
		Type	local
		At	root
		BHVR	Powers/ItemsAndTempPowers/OverheadOffset.bhvr
	End

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeB.part
		Pmagnet	 root
	End

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeC.part
		Pmagnet	 root
	End
End

Condition
	On 	Time
	Time 	83	#89

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeB.part
		Pmagnet	 root
	End
End

Condition
	On 	Time
	Time 	84

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeA.part
		Pmagnet	 root
	End

	Event

		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	3 5 3
			PositionOffset	0 1600 0
		End
	End

	Event

		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		POther	node02

	End

End

Condition
	On 	Time
	Time 	84

	Event

		Ename	node02
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 5 3
			PositionOffset	0 800 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		POther	 node03

	End

End

Condition
	On 	Time
	Time 	84

	Event

		Ename	node03
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 5 3
			PositionOffset	0 300 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 node04

	End

End

Condition
	On 	Time
	Time 	84

	Event

		Ename	node04
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 5 3
			PositionOffset	0 130 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		POther	 node05

	End

End


Condition
	On 	Time
	Time 	84

	Event

		Ename	node05
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 5 3
			PositionOffset	0 30 0
			FadeOutLength		280
		End

	End

####################################################################################################

	Event

		Ename	node06
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride

			FadeOutLength		280
			Startjitter	3 5 3
			PositionOffset	0 -50 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1

		BhvrOverride

			FadeOutLength		280
		End

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 bendmystic

	End

End

######################################################################################

Condition
	On 	Time
	Time 	85

	Event
		EName 	Mallet1
		Type	local
		At	bendmystic	#WepR

		Part	:Reichsman_FlashStreaks.part
		Part	:Reichsman_Flash.part
		Part	:Reichsman_FlashFlat.part

		LifeSpan	5

	End

	Event
		EName 	Mallet1
		Type	local
		At	bendmystic	#WepR
		bhvr	behaviors/ScaleBubble3.bhvr
		BhvrOverride
			Alpha	40
			RGB0 0 0 255
		End
		Geom	GEO_WhiteSphere	#ElectricalMeleeBubble

		LifeSpan	5

	End

	Event
		Type	start
		At	bendmystic	#Root

		Bhvr	Behaviors/StatesmanCameraShake.bhvr

		Part	:ElectricityControl01Lrg.part
		Part	:ElectricityControl02Lrg.part

		Part1	Explosions/FireExplosion/InfernorockBits.part
		Part2	Explosions/FireExplosion/DarkInferno.part
		Part3	Explosions/FireExplosion/DarkFlatInferno.part
		Part	Powers/ElectricityControl/DarkStarCore.part
		LifeSpan	5


	End

	Event
		Type	start
		At	bendmystic	#hips

		Part2	Explosions/FireExplosion/DarkInferno.part
		LifeSpan	5


	End

End

#######################################
#Statesmans aoe lightning
#################

Condition
	On 	Time
	Time 	90

	Event
		Ename	AreaLightning
		Type	local
		At	root
		ChildFX V_COV\EnemyPowers\Reichsman\AreaLightning.fx

	End
End



#Condition
#	On 	Time
#	Time 	90
#
#	Event
#		Ename	Hookup1
#		Type	local
#		At	root
#
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	7 #13
#
#	Event
#		Ename	Hookup2
#		Type	local
#		At	Hookup1
#		BhvrOverride
#			FadeOutLength		280
#			StartJitter		120.0 0.0 120.0
#		End
#
#		geom	Centerdummy
#		LifeSpan	68
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	7	#3
#
#	Event
#		Type	local
#		At	Hookup2
#		Altpiv	1
#
#		BhvrOverride
#			FadeOutLength		280
#			StartJitter		120.0 0.0 120.0
#
#		End
#
#		ChildFx	:Reichsman_LightningRod.fx
#
#
#		LifeSpan	35
#	End
#
#End

#####################################################################
#Statesman's Lightning Bolt
################################################

Condition
	On 	Time
	Time 	84

	Event

		Type	start
		At	node05

		BhvrOverride
			InitialVelocity		0 -10 0
			PositionOffset		0 -10 0
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5


		End
		Cdestroy	1
		CThresh		0.001

	End
End

End