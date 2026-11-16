#########################################################
## Red elec blast hitting target
########################################################
FxInfo

Input  
	Inpname	Chest
End

##LifeSpan	3000000

#################################################################
##Hands
#################################################################
Condition

	On 	time
	Time 	0
	
	Event
		Type	Local
		At	origin
		Sound mu_rumble 80 80 .86
	end
end

Condition

	On 	time
	Time 	0
	
	Event
		Type	Local
	
		At	origin

	Sound chargedarmor_loop 80 80 .5
	end
end

Condition

	On 	cycle
	Time 	1
		Chance	.05
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
		
	End

end



Condition

	On 	cycle
	Time 	5
	Event
		Type	positonly 
	
		At	UarmR
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother LarmR
		pmagnet UarmR
		Lifespan 30
	end

	Event
		Type	positonly 
	
		At	UarmR
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother UarmL
		pmagnet Chest
		Lifespan 30
	end	
	Event
		Type	positonly 
	
		At	UarmL
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother UarmR
		pmagnet Chest
		Lifespan 30
	end

	Event
		Type	positonly 
	
		At	UarmL
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother LarmL
		pmagnet UarmL
		Lifespan 30
	end
	Event
		Type	positonly 
	
		At	UlegR
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother LlegR
		pmagnet UlegR
		Lifespan 30
	end

	Event
		Type	positonly 
	
		At	LlegR
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother FootR
		pmagnet LlegR
		Lifespan 30
	end

	Event
		Type	positonly 
	
		At	chest
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother UlegR
		pmagnet chest
		Lifespan 30
	end

	Event
		Type	positonly 
	
		At	neck
		part1	:mu_Elec_BodyShockArmor1.part
		part1	:mu_Elec_BodyShockArmor2.part
		Pother waist
		pmagnet neck
		Lifespan 30
	end


end


