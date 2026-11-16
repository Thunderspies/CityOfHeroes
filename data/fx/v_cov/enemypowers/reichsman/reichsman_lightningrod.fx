#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	265

########################################################


######################################################################################



Condition
	On 	Time
	Time 	0

	Event
		Ename	offset
		Type	local
		At	root

		BhvrOverride
			StartJitter		40.0 0.0 40.0
		End

		Bhvr	Behaviors/StatesmanCameraShake.bhvr
		geom	Centerdummy

	End

	Event
		Ename	Lightning
		Type	local
		At	offset
		BHVR	Powers/ItemsAndTempPowers/OverheadOffset.bhvr

	End

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeB.part
		Pmagnet	 root

	End

######################################################################################

	Event
		Ename	Lightning2
		Type	local
		At	offset
		BHVR	Powers/ItemsAndTempPowers/OverheadOffset2.bhvr

	End

	Event

		Ename	node01
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 1600 0	#0 150 0
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
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		POther	node02

	End

End

Condition
	On	Time
	Time	10

	Event

		Ename	node02
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
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
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 node03

	End

End

Condition
	On	Time
	Time	0

	Event

		Ename	node03
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
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
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 node04

	End

End



Condition
	On 	Time
	Time 	0
	Event
		Type	local
		At	Lightning2
		Part	:Reichsman_LightningStrikeB.part
		Pmagnet	 root

	End

End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Lightning
		Part	:Reichsman_LightningStrikeA.part
		Pmagnet	 root

	End

#######################################################################################################

	Event

		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 1600 0  #0 50 0
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
	On	Time
	Time	0

	Event

		Ename	node02
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 800 0   #0 40 0
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
	On	Time
	Time	0

	Event

		Ename	node03
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 300 0    #0 30 0
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
	On	Time
	Time	0

	Event

		Ename	node04
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 130 0   #0 20 0
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
	On	Time
	Time	0

	Event

		Ename	node05
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	15 15 15
			PositionOffset	0 30 0   #0 -10 0
			FadeOutLength	280
		End

	End

####################################################################################################

	Event

		Type	start
		At	node05

		BhvrOverride
			InitialVelocity		0 -10 0
			PositionOffset		0 -10 0  #0 -10 0
#			FadeOutLength		280

			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5


		End

		CEvent	Explosions\FireExplosion\Reichsman_LightningFireExplosion.fx	#POWERS\ElectricityControl\LightningRodHit.fx
		Cdestroy	1
		CThresh		0.001

	End


	Event

		Type	start
		At	node05

		BhvrOverride
			InitialVelocity		0 -3 0
#			PositionOffset	0 -10 0
#			FadeOutLength		280

			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5


		End

		CEvent	:LargeFireballBurnHitDecal.fx	#POWERS\ElectricityControl\StatesmanLightningRodHit.fx
		Cdestroy	1
		CThresh		0.00001

	End

####################################################################################################

	Event

		Ename	node06
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride

			FadeOutLength		280
			Startjitter	15 15 15
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
		POther	 Node06

	End

End

######################################################################################

Condition
	On	Time
	Time	0
	Event

		Ename	nodeA
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	.5 3 .5
			PositionOffset	0 1600 0
		End

	End

	Event

		Ename	ChargedBolt1
		Type	start
		At	nodeA
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		POther	nodeB

	End

End

Condition
	On	Time
	Time	0

	Event

		Ename	nodeB
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	.5 3 .5
			PositionOffset	0 800 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	nodeB
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		POther	 nodeC

	End

End

Condition
	On	Time
	Time	0

	Event

		Ename	nodeC
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	.5 3 .5
			PositionOffset	0 300 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	nodeC
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 nodeD

	End

End

Condition
	On	Time
	Time	0

	Event

		Ename	nodeD
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			Startjitter	.5 3 .5
			PositionOffset	0 130 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	nodeD
		altpiv	1

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt02a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt05a.Part
		POther	 nodeE

	End

End


Condition
	On	Time
	Time	0

	Event

		Ename	nodeE
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride
			FadeOutLength		280
			Startjitter	.5 3 .5
			PositionOffset	0 30 0
		End

	End

	Event

		Ename	nodeF
		Type	start
		At	Lightning
		Geom	centerdummy

		BhvrOverride

			FadeOutLength		280
			Startjitter	.5 3 .5
			PositionOffset	0 -50 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	nodeE
		altpiv	1

		BhvrOverride

			FadeOutLength		280
		End

		Part	:Reichsman_LinghtningRodSpark.part
		Part	:Reichsman_LinghtningRodSpark_White.part

		Part	:Reichsman_LightningRod_ChargedBoltCore.Part
		Part	:Reichsman_LightningRod_ChargedBolt01a.Part
		Part	:Reichsman_LightningRod_ChargedBolt03a.Part
		Part	:Reichsman_LightningRod_ChargedBolt06a.Part
		POther	 nodeF

	End


####################################################################################################

	Event

		Type	start
		At	nodeE

		BhvrOverride
			InitialVelocity		0 -3 0
#			PositionOffset	0 -10 0
#			FadeOutLength		280

			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5


		End

		CEvent	Explosions\FireExplosion\Reichsman_LightningFireExplosion.fx	#POWERS\ElectricityControl\StatesmanLightningRodHit.fx
		Cdestroy	1
		CThresh		0.00001

	End

	Event

		Type	start
		At	nodeE

		BhvrOverride
			InitialVelocity		0 -3 0
#			PositionOffset	0 -10 0
#			FadeOutLength		280

			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5


		End

		CEvent	:LargeFireballBurnHitDecal.fx	#POWERS\ElectricityControl\StatesmanLightningRodHit.fx
		Cdestroy	1
		CThresh		0.00001

	End

End