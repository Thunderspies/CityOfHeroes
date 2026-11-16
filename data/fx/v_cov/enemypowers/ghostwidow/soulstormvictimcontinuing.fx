#########################################################
##	Soul Storm Victim Continuing
##

FxInfo

################  Start Glows  #####################




Condition
	On 	Time
	Time 	5
	
	Event
		Type	Local
		At 	origin
		Sound Glow6_loop 88 88 .86
	
	End
End

Condition
	On Time
	Time 5

	Event
		EName 	glow 
		Type	local
		At	waist
		Part	:soulChoke1.part
	End

	Event
		EName 	glow 
		Type	local
		At	chest
		Part	:soulChoke1.part
	End

	Event
		EName 	glow 
		Type	local
		At	Head
		Part	:soulChoke1.part
	End

	Event
		EName 	glow 
		Type	local
		At	Hips
		Part	:soulChoke1.part
	End

End

Condition
	On Time
	Time 10

	Event
		EName 	glow 
		Type	local
		At	chest
		bhvr	behaviors/genericParticleFade.bhvr
		Part	:soulSuck1.part

	End

End

############   Pivot Rig   ##############3


Condition
	On Time
	Time 0

	Event
		EName 	spirit1
		Type	local
		At	Waist
		Bhvr	:soulOrbit2.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit1 
		Type	local
		At	spirit1
		  Altpiv	1
		Bhvr	:soulOrbit2.bhvr
	End
	Event
		EName 	spirit2
		Type	local
		At	Waist
		Bhvr	:soulOrbit2.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit2 
		Type	local
		At	spirit2
		  Altpiv	1
		Bhvr	:soulOrbit2.bhvr
	End

	Event
		EName 	spirit3
		Type	local
		At	Waist
		Bhvr	:soulOrbit2.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit3 
		Type	local
		At	spirit3
		  Altpiv	1
		Bhvr	:soulOrbit2.bhvr
	End

	Event
		EName 	spirit4
		Type	local
		At	Waist
		Bhvr	:soulOrbit2.bhvr
		Geom	helixFXoffset
	End


	Event
		EName 	orbit4 
		Type	local
		At	spirit4
		  Altpiv	1
		Bhvr	:soulOrbit2.bhvr
	End

End


################   P1   ##########################

Condition
	On	Time
	Time	3
	Event
		EName 	Prime
		Type	start
		At	Hips
		Bhvr	:soulProjectile1.bhvr
		Magnet	orbit1
		LookAt	orbit1
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime
		Bhvr	:soulOffset.bhvr
#		Part	:soulGlow1.part
		Part	:soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset1
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset1
		  AltPiv	4
		Part	:soulFire1.part
	End


End


#######################  P2  ####################

Condition
	On	Time
	Time	6
	Event
		EName 	Prime2 
		Type	start
		At	Hips
		Bhvr	:soulProjectile2.bhvr
		Magnet	orbit2
		LookAt	orbit2
	End

	Event
		EName 	Offset2 
		Type	local
		At	Prime2
		Bhvr	:soulOffset3.bhvr
#		Part	:soulGlow1.part
		Part	:soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset2
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset2
		  AltPiv	4
		Part	:soulFire1.part
	End


End


######################## P3 ############################


Condition
	On	Time
	Time	9
	Event
		EName 	Prime3 
		Type	start
		At	Hips
		Bhvr	:soulProjectile1.bhvr
		Magnet	orbit3
		LookAt	orbit3
	End

	Event
		EName 	Offset3 
		Type	local
		At	Prime3
		Bhvr	:soulOffset3.bhvr
#		Part	:soulGlow1.part
		Part	:soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset3
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset3
		  AltPiv	4
		Part	:soulFire1.part
	End

End

#######################   P4   ################################

Condition
	On	Time
	Time	12
	Event
		EName 	Prime4 
		Type	start
		At	Hips
		Bhvr	:soulProjectile2.bhvr
		Magnet	orbit4
		LookAt	orbit4
	End

	Event
		EName 	Offset4 
		Type	local
		At	Prime4
		Bhvr	:soulOffset.bhvr
#		Part	:soulGlow1.part
		Part	:soulFire1.part
		Part	:soulSmoke1.part
		Geom	stormFXsoul
	End


	Event
		EName 	Arm1 
		Type	local
		At	Offset4
		  AltPiv	2
		Part	:soulFire1.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset4
		  AltPiv	4
		Part	:soulFire1.part
	End

End

############  Explode   ################


Condition
	On	Death
	
	Event
		EName 	Explode 
		Type	Start
		At	Waist
		Part	:soulFlash1.part
		Part	:soulExplode1.part		
		Part	:soulSparks1.part
		Part	:soulCloud1.part
		Sound OutofEnergy1 80 80 .7
		Lifespan 1
	End

	Event
		EName 	All 
		Type	Destroy
	End

End

End				