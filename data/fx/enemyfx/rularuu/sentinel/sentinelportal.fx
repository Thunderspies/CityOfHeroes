
FxInfo

Lifespan	30

#Flags    InheritAlpha

##################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest
	
		Part	:PortalAuraStreaks.part
		Part	:AuraEmbers.part
		Part	:PortalFlash.part
		Part	:PortalFlashStar.part
		Part	:NEWPortalEnergyBalls.part

	End

	Event
	
		Type	Local
		At	chest
	
		Part	:PortalAuraStreaks.part
		Part	:NEWPortalAura.part
		Part	:NEWPortalAuraEmbers.part
		Part	:NEWPortalWaves1.part
	
		Lifespan	10
	
	End

End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	chest
	
		Part	:PortalAuraStreaksPurple.part
		Part	:PortalElectricityStreaksPurple.part
			
		Lifespan	20
	
	End

End




End				