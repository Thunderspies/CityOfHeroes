#########################################################
##	chill touch hit
FxInfo

	
#LifeSpan	100

##############################################################
##Glows
######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound tentacle_loop 80 80 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End


Condition
	On	Time
	Time	0

	
	Event	

		ENAME	Base
		Type	local
		At	root

		Part1	:TendrilPuddle.part
		Part2	:TendrilPuddle2.part
		Part3	:SoulTendrilRing.Part
		Part5	:SoulTendrilsEmber.part
		Sound grexplo2 80 80 .6
	End
	
	Event
		EName 	Explode 
		Type	Start
		At	Root
		Part	:soulFlash1.part
		Part	:soulFlash2.part
		Part	:soulExplode1.part
		Part	:soulExplode2.part		
		Part	:soulSparks2.part
		#Sound	OutofEnergy1 80 30 .7
		Lifespan 5
	End

#	Event
#		EName 	glow 
#		Type	local
#		At	UlegR
#		Part	:soulChoke1.part
#		bhvr	behaviors/genericParticleFade.bhvr
#	End
#
#	Event
#		EName 	glow 
#		Type	local
#		At	UlegL
#		Part	:soulChoke1.part
#		bhvr	behaviors/genericParticleFade.bhvr
#	End

	Event
		EName 	glow 
		Type	local
		At	LlegR
		Part	:soulChoke1.part
		bhvr	behaviors/genericParticleFade.bhvr
	End

	Event
		EName 	glow 
		Type	local
		At	LlegL
		Part	:soulChoke1.part
		bhvr	behaviors/genericParticleFade.bhvr
	End

	Event
		EName 	glow 
		Type	local
		At	FootR
		Part	:soulChoke1.part
		bhvr	behaviors/genericParticleFade.bhvr
		Part	:soulSuck1.part
	End

	Event
		EName 	glow 
		Type	local
		At	FootL
		Part	:soulChoke1.part
		bhvr	behaviors/genericParticleFade.bhvr
		Part	:soulSuck1.part
	End

End


##############################################################
##Tenticles01
######################################################

Condition
	On	Time
	Time	4
	
	Event
		EName 	Prime
		Type	Local 
		At	Root 

		Bhvr	behaviors/ScaleTenticle.bhvr

		Anim	FX_tenticlebase
		#Sound	tentacle_loop 80 33 .6
		
	End

#	Event
#		EName 	Prime
#		Type	SetLight
#		At	Prime
#
#		Bhvr	:SoulLights.bhvr
#
#		Lifespan		100
#
#	End

	Event
		EName 	tenticle_1
		Type	Local 
		At	Prime 
		AnimPiv Chest
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_B
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End



	Event
		EName 	tenticle_2
		Type	Local 
		At	Prime 
		AnimPiv Neck
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_3
		Type	Local 
		At	Prime 
		AnimPiv Head
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_A
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_4
		Type	Local 
		At	Prime 
		AnimPiv Hair
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_5
		Type	Local 
		At	Prime 
		AnimPiv Waist
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_A
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End
End

##############################################################
##Tenticles02
######################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Prime2
		Type	Local 
		At	Root 

		Bhvr	behaviors/ScaleTenticleOutside.bhvr

		Anim	FX_tenticlebase
	End


	Event
		EName 	tenticle_11
		Type	Local 
		At	Prime2
		
		AnimPiv Chest
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_B
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_21
		Type	Local 
		At	Prime2 
		AnimPiv Neck
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_31
		Type	Local 
		At	Prime2
		AnimPiv Head
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_A
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_41
		Type	Local 
		At	Prime2 
		AnimPiv Hair
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

	Event
		EName 	tenticle_51
		Type	Local 
		At	Prime2 
		AnimPiv Waist
		Bhvr	behaviors/TentacleFade.bhvr
		Anim	FX_GhostWidow_tentacle_A
		BhvrOverride
			#StartColor	255 200 55
		End
		
		Part	:soulSmokeRing.part
	End

End

################   P1   ##########################
#
#Condition
#	On	Time
#	Time	3
#
#
#	Event
#		EName 	spirit1
#		Type	local
#		At	Root
#		Bhvr	:Spinner01.bhvr
#		BhvrOverride
#			PositionOffset	0 1.5 0
#			#pyrRotate	0 1.5708 0
#			#pyrRotateJitter	.2 0 .2
#		End
#				
#	End
#
#
#	Event
#		EName 	orbit1 
#		Type	local
#		At	spirit1
#		BhvrOverride
#			PositionOffset	0 0 1.5
#		End
#		
#	End
#
#
#	Event
#		EName 	child
#		Type	Local
#		At	orbit1
#		Bhvr	:spinner02.bhvr
#	End
#
#	Event
#		EName 	Offset1 
#		Type	local
#		At	child
#		Bhvr	:soulOffset.bhvr
#		BhvrOverride
#			PositionOffset	0 0 2
#		End
##		Part	:soulGlow1.part
#		Part	:soulFire1.part
#		Part	:soulSmoke1.part
#		Geom	stormFXsoul
#	End
#
#	Event
#		EName 	Arm1 
#		Type	local
#		At	Offset1
#		  AltPiv	2
#		Part	:soulFire1.part
#	End
#
#	Event
#		EName 	Arm2 
#		Type	local
#		At	Offset1
#		  AltPiv	4
#		Part	:soulFire1.part
#	End
#
#
#End
#
#
########################  P2  ####################
#
#Condition
#	On	Time
#	Time	6
#
#
#	Event
#		EName 	spirit2
#		Type	local
#		At	Root
#		Bhvr	:Spinner01.bhvr
#		BhvrOverride
#			PositionOffset	0 2.25 0
#			pyrRotate	0 90 0
#			#pyrRotateJitter	.2 0 .2
#		End
#		
#	End
#
#
#	Event
#		EName 	orbit2 
#		Type	local
#		At	spirit2
#		BhvrOverride
#			PositionOffset	0 0 1.5
#		End
#	End
#
#
#	Event
#		EName 	child2 
#		Type	Local
#		At	orbit2
#		Bhvr	:spinner02.bhvr
#	End
#
#	Event
#		EName 	Offset2 
#		Type	local
#		At	child2
#		Bhvr	:soulOffset3.bhvr
#		BhvrOverride
#			PositionOffset	0 0 2
#		End
##		Part	:soulGlow1.part
#		Part	:soulFire1.part
#		Part	:soulSmoke1.part
#		Geom	stormFXsoul
#	End
#
#
#	Event
#		EName 	Arm1 
#		Type	local
#		At	Offset2
#		  AltPiv	2
#		Part	:soulFire1.part
#	End
#
#	Event
#		EName 	Arm2 
#		Type	local
#		At	Offset2
#		  AltPiv	4
#		Part	:soulFire1.part
#	End
#
#
#End
#
#
######################### P3 ############################
#
#
#Condition
#	On	Time
#	Time	9
#
#
#	Event
#		EName 	spirit3
#		Type	local
#		At	Root
#		Bhvr	:Spinner01.bhvr
#		BhvrOverride
#			PositionOffset	0 3 0
#			pyrRotate	0 180 0
#			#pyrRotateJitter	.2 0 .2
#		End
#		
#	End
#
#
#	Event
#		EName 	orbit3 
#		Type	local
#		At	spirit3
#		BhvrOverride
#			PositionOffset	0 0 1.5
#		End
#	End
#
#
#	Event
#		EName 	child3 
#		Type	Local
#		At	orbit3
#		Bhvr	:spinner02.bhvr
#	End
#
#	Event
#		EName 	Offset3 
#		Type	local
#		At	child3
#		Bhvr	:soulOffset3.bhvr
#		BhvrOverride
#			PositionOffset	0 0 2
#		End
##		Part	:soulGlow1.part
#		Part	:soulFire1.part
#		Part	:soulSmoke1.part
#		Geom	stormFXsoul
#	End
#
#	Event
#		EName 	Arm1 
#		Type	local
#		At	Offset3
#		  AltPiv	2
#		Part	:soulFire1.part
#	End
#
#	Event
#		EName 	Arm2 
#		Type	local
#		At	Offset3
#		  AltPiv	4
#		Part	:soulFire1.part
#	End
#
#End
#
########################   P4   ################################
#
#Condition
#	On	Time
#	Time	12
#
#
#	Event
#		EName 	spirit4
#		Type	local
#		At	Root
#		Bhvr	:Spinner01.bhvr
#		BhvrOverride
#			PositionOffset	0 3.75 0
#			pyrRotate	0 -90 0
#			#pyrRotateJitter	.2 0 .2
#		End
#		
#	End
#
#
#	Event
#		EName 	orbit4 
#		Type	local
#		At	spirit4
#		BhvrOverride
#			PositionOffset	0 0 1.5
#		End
#	End
#
#
#	Event
#		EName 	child4 
#		Type	Local
#		At	orbit4
#		Bhvr	:spinner02.bhvr
#	End
#
#	Event
#		EName 	Offset4 
#		Type	local
#		At	child4
#		Bhvr	:soulOffset.bhvr
#		BhvrOverride
#			PositionOffset	0 0 2
#		End
##		Part	:soulGlow1.part
#		Part	:soulFire1.part
#		Part	:soulSmoke1.part
#		Geom	stormFXsoul
#	End
#
#
#	Event
#		EName 	Arm1 
#		Type	local
#		At	Offset4
#		  AltPiv	2
#		Part	:soulFire1.part
#	End
#
#	Event
#		EName 	Arm2 
#		Type	local
#		At	Offset4
#		  AltPiv	4
#		Part	:soulFire1.part
#	End
#
#End
#

End			