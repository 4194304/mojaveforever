# The Mojave Experiment
The day was Thursday, June 18, 2026. After a few days of contemplating my next move and getting bored with Catalina, I reached a decision. It was time to leave Catalina behind.

I had enjoyed my experience with Catalina. In fact, I thought that my experience with Catalina was great, if not the best operating system I had ever used. However, it was just too usable. I wanted something that would be a challenge...

I opened Disk Utility and formatted the partition of what used to be a failed Vista install. After an hour of screwing around and trying to figure out how I could actually install Mojave on my hard drive, I decided to go with an install USB. Once I was in the installer, I was greeted with multiple deceptive progressbars. I had been no stranger to these after upgrading from Mavericks to Sierra to Catalina on my Late 2013 MacBook Pro, so I knew that the installation would take a long time. As expected, it did eventually finish.

And in I went. I thought that it would be more of an experiment than anything, a retrospective to see how Mojave has aged over the years. I had known that using it would not be very easy. After having to deal with expired certificates and having to bypass software update just to get to 10.14.6, I eventually did.

As I have used it, I have found that some of the apps I use require recompilation, or direct modification.

Here are some of the apps I use and how they have held up:

## Firefox
I use 115 ESR which works perfectly fine for most things. Aside from a few rendering glitches, most web pages work perfectly fine

## CotEditor
I mainly use CotEditor for text editing, and I do find it to be nice of them to put the last compatible release for each macOS version on their website. It also works perfectly fine

## Pixelorama
Works perfectly fine, no complaints or workarounds needed

## Prism Launcher
Similar to Catalina and Windows 10 LTSB 2016, I use Prism Launcher 9.4. Unlike the very outdated 7.2, Prism Launcher 9.4 works perfectly fine and can even do things such as create NeoForge instances.

## Vesktop
This is where things start to become a bit more technical. The last version of the official client to support Mojave is 0.0.297, so using that would not be the best. However, as I had done with Catalina, I compiled Vesktop with an older version of Electron. In contrast to Catalina, Mojave uses Electron 26, a version based on Chromium 116. I followed TK50P's Vesktop legacy instructions, troubleshooted, and worked it down to this:
1. Apply the patches detailed in the guide (change the minimum macOS version from 10.15 to 10.14)
2. Add an override for pnpm to use esbuild 0.19.12 in package.json
3. Remove the entire electronfuses section in package.json
4. Replace lines 88 and 89 in src/main/index.ts with this:
`const enabledFeaturesArray = Array.from(enabledFeatures).filter(Boolean);
const disabledFeaturesArray = Array.from(disabledFeatures).filter(Boolean);`
5. Follow the rest of the guide, replacing the instruction to override with Electron 32 with Electron 26

With all of this working, you will have Vesktop 1.6.5 working flawlessly on macOS Mojave. If you want to use OldCord, I have a theme fix that you can download on this repository (named oldcordfixes.css).

## Steam
Steam is one of the most picky applications on this entire list. I had to use archive.org to load a version of the site from November of 2024, then I could download the installer and launch Steam. I also had to force it into using update resources from an archive.org snapshot, and then it finally worked. While the main client does work, you will have to download games from another computer as Steam has a new compression method that will stall and effectively halt game downloads.

## Geometry Dash
This is likely the most complex part of my entire setup. I had to write an entire shim to get it to work, and it does run well, but there are certain things that can crash it (such as opening a file dialog). Right now, the main game works perfectly fine, Geode works just fine aside from the one crash I mentioned, and I haven't seen it crash in game. If you want to try this yourself, follow the guide in the gdfix folder of this repository. It is not easy.

## iTerm2
iTerm2 also makes things easy, as they tell you the last compatible version similar to CotEditor.

## What do I think?
Overall, Mojave has proven itself to be a perfectly viable daily driver as someone who never gives up and has an affinity for forcing things to work. For any average person however, Mojave is effectively obsolete. A lot of applications have dropped it, but to this day, Mojave can still be great if you force it to.
