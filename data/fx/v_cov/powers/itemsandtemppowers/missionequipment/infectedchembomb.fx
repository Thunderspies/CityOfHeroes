#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	100

Condition

	On Time 
	Time 0

		
	Event
		ENAME	Glow
		Type	SetLight	
		Bhvr	:ChemHitLight.bhvr
		Lifespan 40
	End

	Event
		EName   Splash
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemHitMist.part
		Lifespan	40
	End
End

Condition

	On Time 
	Time 10	
	Event
		EName   Steam
		Type	Posit
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemSmoke.part
		Lifespan	30
	End

End

Condition

	On Time 
	Time 20	
	Event
		EName   Steam
		Type	Posit
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemSmoke02.part
		Lifespan	20
	End

End

Condition

	On Time 
	Time 30
		
	Event
		EName 	Explode
		Type	Local 
		At	Chest
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemSmoke.part
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemHitSplash.part
		#Part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\NovaTendrils.part
		part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemHitFlash.part
		#Part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\ChemFlash.part
		#Part	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\NovaSmoke01.part
		Lifespan	5
	End
		
	Event
		Ename	base
		Type	Start 
		At	Root
	End

#	Event
#		EName 	Prime8
#		Type	Local 
#		At	base
#		bhvr	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\BlastRingScale.bhvr
#		Part1   V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\Infernoflash.part
#		Part2   V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\Infernoflash2.part
#	End

#	Event
#		EName 	crime
#		Type	Local
#		At	base
#		Part2	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\Inferno.part
#		Part2	V_COV\Powers\ItemsandTempPowers\Warburg\ChemicalWeapon\Inferno.part
#		Lifespan	20
#
#	End
#
End


Condition

	On Time 
	Time 30
	Repeat	11
		
	Event
		EName 	Explode
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		
	End

End

Condition

	On Time 
	Time 30
	Repeat	4
	
	Event
		EName 	Explode
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonQuillDarkScatter.fx
		
	End

End

End
