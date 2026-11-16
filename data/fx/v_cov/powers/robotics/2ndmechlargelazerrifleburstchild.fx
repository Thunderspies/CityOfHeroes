#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	33


#	Event
#		Type	Local
#		At	WepR
#		BHVR	:BlastOffset.bhvr
#		Part	:BlastOriginStar.part
#		Part	:BlastOriginEnergy.part
#		Part	:BlastOriginEnergyRed.part
#		LifeSpan	20
#	End

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	centerdummy
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:LargeVillianSniper_Tracer.part
#		Part	:LargeVillianSniper_Tracer.part
#		Part	:LargeVillianSniper_Tracer.part

		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		Part	:LargeVillianSniper_flare.part

		Part	:LargeVillianSniper_Flare2.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastCore.part
#		Part	:LargeEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End
	
End

##################################


Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

End				