#########################################################
##	template

FxInfo


##Lifespan 60

#######  barrel  ####################################


Condition
	On 	Time
	Time	0

	Event
		EName 	Gnome
		Type	Local
		At	Chest
		Anim	FX_goblin_smother
		Bhvr	Behaviors/smother_offset.bhvr
	End


End

#####  child fx  #############################

#Condition
#	On	Death
#
#
#
#
#	Event
#		
#		Type	local 
#		At	Origin	
#		ChildFx		Explosions\TurretExplosion\LargeTurretExplosion.Fx
#		
###		Lifespan 15
#	End
#
#End

########  explosion  ################################

Condition
	On	Death


	Event
		
		Type	local  ##Posit
		At	Chest
		
		Part	Explosions\medium\SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Sparks
		Type	local  ##Posit
		At	Chest
		
		Part1	Explosions\medium\FireExplosion2.part
		Part1	Explosions\medium\GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		
		Lifespan 8
	End

	Event
		EName 	TankBlast_flash
		Type	local  ##Posit
		At	Chest
	
		Part	Explosions\medium\FireExplosionFlash.part
		Lifespan 5
		
	End

End


End		