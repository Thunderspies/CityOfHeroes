
-- This starts the normal cutscene.
cutsceneStart = [====[
			{
							< 0 Camera
							Position Camera0
							MoveTo Camera2
							Target Actor1
							Over 30
							>
							< 0 Actor2 "<em AtEase>"
							>
							< 5 Actor1 "<em bow t=5 d=5>Hello, citizens of the Rogue Isles. My name is Wu Yin."
							>
							< 0 SoundFX
								Music Cutscene_ImperialPower_Music
							>
							< 5 Actor1 "<em thanks t=5 d=5>I am broadcasting this message to you all to put forth a bounty."
							>
							< 5 Actor1 "{HeroName} is currently housing Praetorian criminals."
							>
							< 5 Actor1 "<em no t=5 d=5>Whoever brings me {HeroName}, alive, will be given twenty million dollars. -Only- if {Hero.gender=male he|she} is alive."
							>
			]====]

-- If the player hasn't done anything special, they will see cutsceneYin
cutsceneYin = [====[
							< 5 Actor1 "<t=5 d=5>Be careful, as {HeroName} has gathered several Praetorian allies."
							>
							< 5 Actor1 "<t=5 d=5>It will be extremely dangerous, as {Hero.gender=male he|she} will use them to the best of their abilities."
							>
							< 5 Actor1 "<em welcome t=5 d=5>The bounty is set. We will be watching."
							>
			]====]

-- If the player has the objective, ScriptDeanAppears, then the cutscene changes to this.
cutsceneDean = [====[
							< 3 Actor1 "<t=3 d=3>Be careful, as {HeroName} has -"
							>
							< 5 Caption "<x=0.5 y=0.2 t=6 d=5><scale 1.75><color Olive><bgcolor DeepSkyBlue>Can't let you go on, Yinny!"
							>
							< 5 Actor1 "<em no t=4 d=4>What?! Who is interrupting this?"
							>
							< 0 SoundFX
								Music i24_Villain_DeanMacArthur_CutsceneMusic_01
							>
							< 0 Camera
								Position Camera1
								DepthOfField 10
							>
							< 5 Actor3 "<em victory t=5 d=5>The ONE, the ONLY, Dean MacArthur! D-Mac, to the ladies."
							>
							< 0 Camera
								Position Camera3
								DepthOfField 3
							>
							< 8 Actor3 "<em explain t=8 d=8>Now, {HeroName} and I are old friends, {Hero.gender=male wartime chums|the kind with romantic tension}! You mess with {Hero.gender=male him|her}, you mess with the Macster!"
							>
							< 0 Camera
								Position Camera4
								DepthOfField 8
							>
							< 5 Actor3 "<em no t=5 d=5>Anyone out there thinking about claiming this bounty should just walk away, 'cause {HeroName} will take you down!"
							>
							< 0 Camera
								Position Camera1
								DepthOfField 10
							>
							< 5 Actor3 "<em point 5= d=5>And then {Hero.gender=male he's|she's} gonna come after you, Yin-boy, and you ain't gonna be able to handle that perfect storm!"
							>
							< 3 Caption "<x=0.5 y=0.2 t=3 d=3><scale 1.75><color white><bgcolor black>Cut the feed!"
							>
			]====]

-- This will end the cutscene.
cutsceneEnd = [====[
			< 0 Actor1 ""
					Action DestroyMe
			>
			< 0 Actor2 ""
					Action DestroyMe
			>
			< 0 Actor3 ""
					Action DestroyMe
			>
			}
			]====]

-- This function calls out which cutscene should play, depending on if the mission objective, ScriptDeanAppears, is true.
function tick()
	if Mission.ObjectiveIsSucceeded("ScriptDeanAppears") then
		cutscene = cutsceneStart .. cutsceneDean .. cutsceneEnd
	else
		cutscene = cutsceneStart .. cutsceneYin .. cutsceneEnd
	end

	Encounter.StartCutSceneFromString(cutscene, Encounter.MyEncounter())
	EndScript()
end
