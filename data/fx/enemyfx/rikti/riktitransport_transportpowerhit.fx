#########################################################
##	template

FxInfo

LifeSpan	250

#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End

######################################################

Condition

	Event
		Ename	TargetNodeRight
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -60 126
		End
		
		Part	:BlinkingLights.part
		LifeSpan	25
	End

End

##########################################################
#######################Offset Points Right ##################
##########################################################

Condition

	Event
		Ename	TargetNodeLeft
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -60 -126
		End
		
		Part	:BlinkingLights.part
		LifeSpan	25
	End

End

#########################################################
################## Reverse Hit VFX #######################
#########################################################

#
#Condition
#	On	Time
#	Time	0
#	
#	Event
#		Ename	Prime6
#		Type	start
#		At	Target
#		bhvr	:projectile.bhvr
#		
#		Part	:ProjectileEnergyStreak.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		
#		Magnet	TargetNodeRight
#		LifeSpan	250
#
#	End
#	
#	Event
#		Ename	Prime7
#		Type	start
#		At	Target
#		bhvr	:projectile.bhvr
#		
#		Part	:ProjectileEnergyStreak.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		Part	:ProjectileEnergyGasses.part
#		Part	:ProjectileEnergyGasses2.part
#		
#		Magnet	TargetNodeLeft
#		LifeSpan	250
#
#	End
#
#End
#
#Condition
#	On	Prime6Hit
#
#	Event
#		Ename	Prime6
#		Type	Destroy
#	End
#
#End
#
#Condition
#	On	Prime7Hit
#
#	Event
#		Ename	Prime7
#		Type	Destroy
#	End
#
#End


#########################################################
################## Hit VFX Gasses#######################
#########################################################
	
Condition

	Event
		EName 	crime
		Type	start
		At	Root
		
		Part2	:ChemMist.part
		Lifespan	240

	End
	
	Event
		Ename	Start
		Type	Start 
		At	Root
		Bhvr	:CloudRiser.bhvr
	End

End

Condition

	On 	Time
	Time 	30

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part	:InfernorockBits1.part
		Lifespan	100

	End

End

Condition

	On 	Time
	Time 	20

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part	:InfernorockBits2.part
		Lifespan	90

	End

End

Condition

	On 	Time
	Time 	10

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part	:InfernorockBits3.part
		Lifespan	80

	End

End

Condition

	On 	Time
	Time 	40

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part2	:Inferno.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	30

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part2	:Inferno1.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	20

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part2	:Inferno2.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	12

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part2	:Inferno3.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	5

	Event
		EName 	crime
		Type	start
		At	Start
		
		Part2	:Inferno4.part
		Lifespan	20

	End
	
	Event
		EName 	crime
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	0 40 0
		End

		Part2	:HitEnergyStreaksUp.part

		Lifespan	140

	End
######################################################################################
##Hit Core
#######################################

	Event
		Type	start
		At	chest
		
		Part	:HitEnergyFilliments.part
		Part	:HitEnergyGasses.part
		Part	:HitEnergyGasses2.part

		Lifespan	10
		
	End
	
	Event
		Type	start
		At	chest
		

		Part	:HitEnergyStreak.part

		Lifespan	5
		
	End
End


###################################################################
################Splashes
#######################################

Condition

	Event
		EName   Splash
		Type	start
		At	Start
		ChildFX	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemSplash.fx
		Lifespan	90	#100
	End

	Event
		EName   Splash2
		Type	start
		At	Start
		ChildFX	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemSplash.fx
		Lifespan	150	#160
	End

End

###################################################################
################Camera shake on hit
#######################################

Condition

	Event
		Type	start
		At	chest
		
		Bhvr	Behaviors/CameraShake01.bhvr

		Lifespan	1
		
	End

End

End	

#########################################################

	